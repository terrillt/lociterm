# LociTerm System — SK Architecture Role

**Last Updated**: 2026-08-25

How the SK fork of LociTerm fits the SKMUD architecture, and the
facts an implementer needs. Upstream protocol specs live beside this
file in `docs/` (upstream-owned — see the docs boundary in AGENTS.md).

## Role

The SK **browser gateway**: one `locid` process serves the PWA client
bundle over HTTP and bridges WebSocket ⇄ telnet to the MUD. Runs as a
**parallel proxy** beside MUDitM — never chained through it:

```
Native client  → TLS → MUDitM  → telnet → merc      (port 2026 → 2027)
Browser client → ws  → locid   → telnet → merc      (port 4005 → 2027)
```

Chaining was evaluated and rejected (2026-08-25): the one capability
that matters — real client-IP attribution — is knowable only at the
WebSocket edge, so it must originate in locid regardless; a MUDitM hop
would add only a plaintext-localhost TLS/MCCP layer, and an active
MCCP stream flips MUDitM pattern-blind exactly where GMCP bring-up
wants visibility.

## Process Shape

Single process, event-driven (libwebsockets on libuv — kqueue on
Darwin, no epoll anywhere). One listening port serves both the static
client bundle and the WebSocket. Each browser tab is an independent
telnet connection to merc, indistinguishable from a native client.
Telnet is spoken via vendored libtelnet (real state machine — parses
and re-emits, not a byte relay), which makes locid a **strict-parser
interop check** on anything the MUD sends.

## GMCP Relay Properties (verified by source read, 2026-08)

- **Passive negotiation**: `WONT`/`DO` posture — the MUD must send
  `IAC WILL GMCP` first, or GMCP never comes up.
- **Transparent both directions**: subnegotiation payloads relayed
  verbatim (1-byte ws opcode prefix, `GMCP_DATA=7`); unknown packages
  (e.g. `SK.*`) reach the browser untouched.
- **Client side**: unknown modules are **counted** in the netstat
  panel's per-module table — zero-code end-to-end validation for new
  server packages. `gmcp.js` `inlineDebug` flag traces every GMCP
  message in the terminal.
- **Gotcha**: browser→MUD GMCP is silently dropped (no log) unless
  the option negotiated on at least one side.
- **Server-driven UI**: upstream ships `Loci.Menu`, `Loci.Hotkey`,
  `Client.Media`, `Char.Login` modules — the game can push menus,
  hotkeys, and audio to the stock client. `client/src/gmcp/template.js`
  is the documented base for an SK module.

## Feature Environment Deployment

See `docs/project_notes/key_facts.md` for the live values (host, URL,
config, log, build prefix). Summary: `server/locid-feature.conf`,
plain ws on 4005 (tailnet-only), `engine = none`, game at
`127.0.0.1:2027`. Launched manually; startup-watchdog integration is
an open roadmap item.

## Build

Full recipe in AGENTS.md. The two facts people will forget:
libwebsockets must be **source-built** (brew's lacks the libuv
backend) and **shared, never static** — upstream's `lwsl_timestamp`
interposition in `debug.c` duplicate-symbol-collides against a static
archive on every platform — and the make line needs
`LDFLAGS="-Wl,-rpath,<prefix>/lib"` (which also sidesteps Intel-era
`/usr/local/lib` fossils).
