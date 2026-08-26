# Server Changelog

Runtime behavior changes to locid and the web client. Append-only history.

## Entry Format

```
- [x] `SK-VER` `YYYY-MM-DD` Brief description — details. **Files:** `file.c`
```

---

- [x] `6.0.0` `2026-08-24` No runtime behavior changes in the first pass — the macOS port's only functional touch is the netstat RTT fields on Darwin (`tcp_connection_info` values scaled ms→µs to preserve the JSON contract; zeroes were the alternative). Everything else is build-time. **Files:** `server/src/proxy.c`
