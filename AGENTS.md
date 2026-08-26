# LociTerm -- SKMUD Fork

Browser-to-MUD gateway: WebSocket/PWA client (xterm.js) + telnet proxy
daemon (`locid`). Forked from
[RahjIII/lociterm](https://github.com/RahjIII/lociterm) (v2.16.1,
same author as MUDitM, LGPL-3.0-or-later). This fork adds macOS build
support and SKMUD feature-environment configuration; it serves as the
SK browser gateway and the GMCP validation surface.

**Docs boundary (IMPORTANT):** upstream owns `docs/` content that
predates the fork -- protocol specs (`loci.menu.txt`,
`loci.hotkey.txt`, `mud-mode.txt`, ...) and `docs/dbupgrades/`.
NEVER modify those files on the `feature` branch. SK documentation is
purely additive: this file, `CLAUDE.md`, `docs/lociterm-system.md`,
and `docs/project_notes/`. Ownership is answered by
`git diff main...feature`, not by directory layout.

**See also:**
- `docs/lociterm-system.md` -- SK architecture role, build recipe, GMCP relay properties, deployment
- `docs/project_notes/key_facts.md` -- Ports, paths, build requirements, branch strategy
- `docs/project_notes/bugs.md` -- Active bugs and investigations
- `docs/project_notes/roadmap.md` -- Planned work (SK proxy parity, upstream PRs)
- `docs/project_notes/pull-requests.md` -- PR tracking for upstream submissions
- `docs/project_notes/server-changelog.md` -- Runtime behavior changes
- `docs/project_notes/infra-changelog.md` -- Build system and deployment changes
- `docs/project_notes/test-changelog.md` -- Test work

## Branch Strategy

SKMUD convention (`main` = upstream; SK work uses SKMUD branch
names), with one quirk: upstream's own default branch is named `dev`.

- `main` -- upstream lineage (mirrors upstream's `dev`).
  Upstream-clean; PR-worthy commits get cherry-picked to topic
  branches off `main` for submission.
- `feature` -- SK branch, named for the SKMUD branch whose work it
  carries: macOS port + SK config + SK docs overlay. Never submitted
  upstream as-is.
- PR numbering: `PR-LOCITERM-NNN` in `docs/project_notes/pull-requests.md`.

## Versioning

`LOCITERM_CORE` is upstream's (moves only via upstream sync). The
fork's layer is versioned through upstream's own META mechanism:
`META_NAME = SK`, `META_VERSION = <SKMUD release>` in the root
makefile — effective version `2.16.1-SK.6.0.0`. Changelog tags use
the SK release number. Full scheme (including the standalone-build
caveat and the browser cache-bust bonus):
`docs/project_notes/key_facts.md` § Versioning.

## Build (macOS, feature env)

libwebsockets MUST be built from source -- Homebrew's formula lacks
the libuv event backend LociTerm requires:

```bash
# one-time: lws 4.5.8 SHARED into a prefix — shared matches upstream's
# own install (his lwsl_timestamp interposition in debug.c only works
# against a shared lws; a static archive is a duplicate-symbol error
# on EVERY platform — learned the hard way, 2026-08-25)
git clone --depth 1 --branch v4.5.8 https://github.com/warmcat/libwebsockets
cmake -DCMAKE_INSTALL_PREFIX=$HOME/lociterm-build/lws-prefix \
  -DLWS_IPV6=ON -DLWS_WITH_ZLIB=ON -DLWS_WITHOUT_EXTENSIONS=OFF \
  -DLWS_WITH_LIBUV=ON -DLWS_WITH_EVENT_LIBS=ON -DLWS_WITH_SYS_ASYNC_DNS=0 \
  -DLWS_WITHOUT_TESTAPPS=ON \
  -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3 .. && make -j8 install

# server daemon (rpath so the prefix .so/.dylib is found at runtime)
cd server/src
PKG_CONFIG_PATH=$HOME/lociterm-build/lws-prefix/lib/pkgconfig:/opt/homebrew/lib/pkgconfig \
  make CC=cc LDFLAGS="-Wl,-rpath,$HOME/lociterm-build/lws-prefix/lib"
# overriding LDFLAGS also sidesteps stale x86_64 libs in /usr/local/lib

# web client (pure JS, no native modules)
make client   # from repo root; npm install + webpack, stages dist/var/www/lociterm
```

**PR hygiene note:** the client build runs `npm version
--allow-same-version`, which rewrites `client/package.json` on every
build -- never let that churn into a commit.

## Run (feature env)

```bash
cd server/src
./build/locid -c ../locid-feature.conf     # serves http://skmud-feature:4005
```

Config: `server/locid-feature.conf` -- plain ws (tailnet is the
door), `[game-db] engine = none`, game wired direct to merc:2027
(parallel proxy beside MUDitM, deliberately NOT chained -- see
`docs/lociterm-system.md`).

## Project Notes

Same conventions as SKMUD: check `docs/project_notes/bugs.md` and
`roadmap.md` before starting work; changelog entries on completion;
PR-worthy vs SK-specific commit separation is mandatory.
