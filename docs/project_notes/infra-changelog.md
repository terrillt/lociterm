# Infrastructure Changelog

Build system, deployment, and project structure changes. Append-only history.

## Entry Format

```
- [x] `SK-VER` `YYYY-MM-DD` Brief description — details. **Files:** `file.c`
```

---

- [x] `6.0.0` `2026-08-25` Feature-env deployment on skmud-feature — locid built and serving http://skmud-feature:4005 (tailnet-only): libwebsockets 4.5.8 source-built SHARED into `~/lociterm-build/lws-prefix` (brew formula lacks the required libuv backend; an initial static build was corrected same day — static lws duplicate-symbol-collides with upstream's `lwsl_timestamp` interposition on every platform, and shared matches upstream's own install); server built via `make CC=cc LDFLAGS="-Wl,-rpath,<prefix>/lib"` (overriding LDFLAGS also defeats stale x86_64 fossils in `/usr/local/lib`); client webpack-built via `make client`; game wired direct to merc:2027 as a parallel proxy beside MUDitM (deliberately not chained — attribution must originate here; chaining adds a pattern-blind MCCP hop). x86_64 verification build (gcc 15, zero warnings) in `~/skmud-lociterm-check/` on the prod host. **Files:** `server/locid-feature.conf` (new)
- [x] `6.0.0` `2026-08-25` macOS port (commit `346f0df`) — one real code blocker: `struct tcp_info` absent on Darwin; shimmed via new shared header `loci_tcpinfo.h` with ms→µs scaling (netstat-only). Makefile portability: lazy clang/gcc warning-flag selection, `$(origin CC)` guard, ctags tolerated. `debug.c` untouched — an earlier draft patched its `lwsl_timestamp` override, but the collision was our static-lws build divergence; building lws shared per upstream needs no change. Code-reviewed; tracked as PR-LOCITERM-001. **Files:** `server/src/{loci_tcpinfo.h,client.h,game.h,client.c,game.c,proxy.c,makefile}`
- [x] `6.0.0` `2026-08-24` Fork established — `terrillt/lociterm` from upstream `dev` @ `6370cd5` (upstream's one and only branch), added as SKMUD submodule at `src/externals/lociterm/` per ADR-015 discipline. Branches per SKMUD convention: `main` = upstream lineage, `feature` carries port + config + this docs overlay; upstream `docs/` content untouched by rule. **Files:** `AGENTS.md`, `CLAUDE.md`, `docs/project_notes/` (new)
