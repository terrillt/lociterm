# Pull Requests

Tracking upstream PR submissions to [RahjIII/lociterm](https://github.com/RahjIII/lociterm).

Numbering is `PR-LOCITERM-NNN`, chronological by commit order. One PR
per commit. PR-worthy commits touch only upstream code (no SK config,
no SK docs) and are cherry-picked from `feature` to a topic branch
off `main` for submission.

## Status Legend

- **Pending** -- on `sk`, needs cherry-pick to a topic branch off `dev`
- **Ready** -- on a topic branch, PR not yet submitted
- **Submitted** -- PR open on upstream
- **Merged** -- accepted upstream
- **Declined** -- rejected or withdrawn

---

## PR-LOCITERM-001: macOS build portability

- **Status**: Pending
- **Commit**: `346f0df` (feature, 2026-08-25)
- **Description**: Darwin has no `struct tcp_info` -- new shared
  header `loci_tcpinfo.h` (included by `client.h` and `game.h`,
  registered in `ADDITIONAL_HFILES`) maps to
  `struct tcp_connection_info` / `TCP_CONNECTION_INFO` with ms-to-us
  scaling so the netstat JSON contract is preserved; accessor macros
  used in `client.c`, `game.c`, `proxy.c`. On Linux the shim resolves
  to `tcp_info` unchanged. Makefile: clang-vs-gcc warning flag
  selection (`-Wno-discarded-qualifiers` is GCC-only), `CC ?= cc` via
  `$(origin)` guard, `ctags` failure no longer breaks the default
  build. `debug.c` untouched (an earlier draft patched its
  `lwsl_timestamp` override -- the collision was our own static-lws
  build divergence, not a platform issue; building lws shared per
  upstream's INSTALL needs no change). 7 files, +70/-14. Verified:
  zero-warning build on x86_64 gcc 15 and arm64 Apple clang 21
  (clang: two pre-existing upstream -Wparentheses-equality warnings),
  linked against shared lws 4.5.8, binary runs on both. Code-reviewed
  2026-08-25: Darwin field mapping confirmed against the Apple SDK
  (tcpi_srtt is the ms-domain smoothed-RTT analogue), macro aliasing
  preprocessor-identical on Linux; header preamble matched to house
  style per review.
- **Upstream value**: opens macOS as a dev/deploy platform; the
  tcp_info shim is the only functional change and preserves JSON
  units.

## PR-LOCITERM-002: Standalone server builds derive the real version

- **Status**: Pending
- **Commit**: `b0f8c56` (feature, 2026-08-25)
- **Description**: `server/src/makefile` standalone builds reported a
  hardcoded stale fallback (`2.9.0-dev<ts>`). Now derives
  `LOCITERM_CORE` from the toplevel makefile via sed when present
  (yielding e.g. `2.16.1-dev<ts>` — the `-dev` suffix still marks
  not-the-blessed-full-build), falling back to the old hardcoded
  string only when the toplevel isn't found. Addresses the makefile's
  own "Ideally, this should match" comment. Scrape hardened per
  2026-08-25 code review: tolerates `:=`/`?=` assignment styles and
  trailing comments, first-match only — a toplevel reformat degrades
  to the fallback rather than silently capturing garbage. Verified on
  x86_64 gcc and arm64 clang clean builds, plus a five-case sed
  matrix (`=`, `:=`, `?=`, trailing comment, no-match).
- **Upstream value**: correct version strings in logs/MSSP for anyone
  building only the server.
