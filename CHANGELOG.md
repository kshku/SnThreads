# Changelog

## [0.2.0] - 2026-06-29

### Changed
- Updated the dependency versions

## [0.1.0] - 2026-06-11

- First release. See [0.0.0] section in CHANGELOG.md for full changelog.

## [0.0.0] - 2025-12-28

### Added
- Thread creation, detach, join, exit, and self-identification
- Mutex with lock/try_lock/unlock
- Read-write lock with read/write/try_lock support
- Condition variable with wait, timed-wait, signal, broadcast
- Counting semaphore
- Spinlock with pause hint
- Full atomic operations (load, store, exchange, CAS, fetch_add/sub/or/xor/and) for all 32 standard types
- Memory fence with configurable ordering
- POSIX pthreads backend
- Windows backend
- x86-64 inline assembly (GCC/Clang) and MASM (MSVC) for atomics
- ARM64 inline assembly (GCC/Clang) and ARMASM (MSVC) for atomics
- Shared atomics infrastructure with per-architecture dispatch
- SnCore dependency
- CI workflows (Linux, macOS, Windows, formatting)
