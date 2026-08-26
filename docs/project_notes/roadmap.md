# Roadmap

Open work for the SK lociterm fork. Completed items move to a changelog.

## SK Proxy Parity

- [ ] MNES audit + real-IP forwarding — browser connections currently reach merc as bare localhost: no `IPADDRESS`/`TRUSTED_IPADDRESS`, so `terminals` shows no client identity and **proxy ban checks cannot apply to browser users**. AUDIT UPSTREAM FIRST (real libtelnet stack; may already send some NEW-ENVIRON vars) then close gaps on `sk`: TRUSTED_IPADDRESS from the WebSocket peer address, `PROXY_NAME` ("LociTerm-x.y"), `SECURITY` (ws vs wss), `COMPRESSION`. Mirrors the MUDitM SK layer; makes locid a `PROXY_ALLOWED` trusted source. Revisit scope after SKMUD GMCP Phase 1 — `Core.Hello` may carry part of this
- [ ] Connection limiting — MUDitM's max-children equivalent; check upstream's libwebsockets-level options before porting anything

## Integration

- [ ] `startup` watchdog integration — locid is launched manually; adding it beside MUDitM/Discord/SKALD in SKMUD's `startup` is a shared-script change (user decision pending)
- [ ] SKMUD test harness coverage — `test_server_lociterm_*.py` server-chain tests (ws connect, greeting through the relay, GMCP round-trip once Phase 1 lands)
- [ ] TLS option — if PWA/clipboard secure-context features are ever wanted over the tailnet, `tailscale serve` can front 4005 with real certs (no locid config change)

## GMCP (SKMUD client strategy)

- [ ] SK client module — `client/src/gmcp/template.js` is the documented starting point for an `SK.*` handler module (+ one `initModule` line in `gmcp.js`); needed for modules to appear in `Core.Supports.Set`. Until then, unrecognized `SK.*` packages are counted in the netstat panel — the zero-code validation path for GMCP Phase 1-2
- [ ] `Loci.Menu` / `Loci.Hotkey` theming — server-driven menu/hotkey push (tier-2 adjacent: SK value through the stock client; specs in upstream `docs/`)

## Upstream

- [ ] Submit PR-LOCITERM-001 (macOS port) + PR-LOCITERM-002 (standalone version derivation) after feature-env stability proves out. See pull-requests.md
