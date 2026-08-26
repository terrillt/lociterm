# Key Facts

Ports, paths, versions, and constraints for the SK lociterm fork.
Never store secrets here.

## Upstream

- **Repo**: github.com/RahjIII/lociterm (Jeff Jahr / The Last Outpost — same author as MUDitM)
- **Version at fork**: 2.16.1 (`LOCITERM_CORE` in root makefile); HEAD `6370cd5`, 2026-02-02
- **License**: LGPL-3.0-or-later (headers + NOTICE govern; `client/package.json` says bare "GPL", imprecisely)
- **Default branch**: `dev` (upstream's convention — our branch strategy adapts; see AGENTS.md)
- **libtelnet**: vendored (Sean Middleditch v0.23 fork — deliberately not Debian's 0.21)

## Feature Environment Deployment

- **Host**: skmud-feature (MacBook, tailnet-only)
- **URL**: http://skmud-feature:4005 — one port serves HTTP (client bundle) + WebSocket
- **Config**: `server/locid-feature.conf` — `security = none` (plain ws; the tailnet is the door), `[game-db] engine = none` (single-MUD gateway, no public directory; avoids the sqlite schema-version hard-exit)
- **Game wiring**: direct to merc `127.0.0.1:2027` — parallel proxy beside MUDitM, deliberately NOT chained (real-IP attribution must originate here regardless; chaining adds a pattern-blind MCCP hop; matches the modernization doc's parallel-proxy architecture)
- **Log**: `/Users/mud/SKMUD/log/lociterm.log`
- **Launch**: manual (`./build/locid -c ../locid-feature.conf`) — NOT under SKMUD `startup`'s watchdog (adding it is a shared-script change; open roadmap item)
- **PWA**: installable from the URL; mobile-first; reconnects across client IP changes

## Versioning

- **`LOCITERM_CORE`** (root makefile) is upstream's semver — owned by Jeff, moves only via upstream sync (accepted PRs land upstream first and arrive back the same way). Never modify it.
- **`META_NAME = SK` / `META_VERSION = <SKMUD release>`** — upstream's designed extension point for downstream forks. META_VERSION is bumped alongside SKMUD releases whenever this fork changed in that cycle. Effective version string: `2.16.1-SK.6.0.0`.
- **Bonus**: browser-cached client code detects META_VERSION changes and force-reloads — SK release bumps bust player caches automatically.
- **Changelog tags use the SK release number** (e.g. `6.0.0`), per the deployed-release tagging rule; the upstream base is visible here and in the version string.
- **Standalone `server/src` builds** derive `LOCITERM_CORE` only (no META decoration): they report `2.16.1-dev<ts>` — correct, since a standalone dev build isn't a blessed SK release.

## Build Requirements (macOS)

- **libwebsockets**: MUST be source-built (brew formula lacks the libuv backend) and MUST be SHARED — upstream's `debug.c` `lwsl_timestamp` interposition only works against a shared lws; a static archive is a duplicate-symbol link error on every platform (verified x86_64 + arm64, 2026-08-25). v4.5.8, flags in AGENTS.md. Prefix: `~/lociterm-build/lws-prefix` (laptop), `~/skmud-lociterm-check/lws-prefix` (prod check dir)
- **rpath on the make line**: `LDFLAGS="-Wl,-rpath,<prefix>/lib"` so the prefix `.so`/`.dylib` resolves at runtime; overriding LDFLAGS also sidesteps stale x86_64 Intel libs in `/usr/local/lib` (pre-Apple-Silicon Homebrew fossils)
- **Other deps** via pkg-config/brew: glib-2.0, json-c, sqlite3, libzstd, libuv, zlib
- **Client**: node/npm (pure JS — xterm.js 5.5 + webpack 5, zero native modules). Root `package-lock.json` is an empty decoy; the real lockfile is `client/package-lock.json`

## Constraints

- **Version strings**: see the Versioning section above; standalone-build derivation fixed first pass (PR-LOCITERM-002).
- **GMCP posture**: passive (`WONT`/`DO`) — the MUD must offer `IAC WILL GMCP` first or GMCP never negotiates. Client→MUD GMCP is silently dropped pre-negotiation.
- **npm version churn**: every client build rewrites `client/package.json` — keep it out of commits.
