# Changelog

## [0.2.2] - 2026-09-28

### Fixed
- Fix the AMD64 compare exchange hardcoding a 64 bit rax, so every width
  through it read and wrote 8 bytes. An int32_t atomic on the stack had the
  4 bytes past it overwritten, which the compiler reported as a smashed stack
- Fix the ARM64 atomics using a plain exclusive access at every width, so an
  int8_t or int16_t atomic was read and written 8 bytes at a time. On a stack
  object that is an alignment fault, and on anything else it silently corrupts
  the neighbouring bytes. Each width now has its own load, store, exclusive
  load, exclusive store and compare exchange, and the assembly operand width
  is matched to the object rather than left to the compiler, which also clears
  the 324 asm operand width warnings the ARM64 build emitted
- Fix the ARM64 assembly subtracting by adding, and exclusive or-ing by
  exclusive or-ing, because sn_atomic_fetch_sub, sn_atomic_fetch_xor and
  sn_atomic_fetch_and reused the add and or macros
- Fix sn_atomic_compare_exchange_explicit, which called a function named
  get_generic_atomic_function. Nothing declares it, so any use of the macro
  failed to compile. Every other generic macro goes through
  SN_GET_GENERIC_ATOMIC_FUNCTION
- Fix sn_atomic_fetch_or, sn_atomic_fetch_xor and sn_atomic_fetch_and returning
  the value they had just stored instead of the one that was there. The compare
  exchange loop they are built on tested the wrong condition, so it kept going
  after a successful swap and the retry overwrote the result with the new
  value. The stored value was always correct
- Fix the store in the AMD64 atomics using the %z1 operand modifier, which is a
  GNU as extension that clang rejects outright. Anything built for x86-64 with
  clang, which is every Intel Mac, could not compile this library at all. CI
  did not catch it because macos-latest is ARM64 and never took this path
- Make sn_atomic_flag_clear_explicit a read modify write, an exchange on AMD64
  and a store release on ARM64, instead of a plain store that could be
  reordered against a concurrent test and set
- Honour the memory order sn_atomic_flag_test_and_set_explicit is given,
  instead of discarding it. Both implementations already emitted a full
  barrier, so this only makes the contract true
- Mark the flag load asm volatile and give it a memory clobber. The volatile
  flag member already kept the load in place, so this is hardening rather than
  a fix

### Added
- Cover every generic atomic macro with a test, plus the atomic flag, its
  single winner guarantee and the write back on a failed compare exchange.
  Narrow types get their own test, which is what exposes the width faults
  above, so a regression names the operation that broke rather than failing
  somewhere downstream of it

## [0.2.1] - 2026-09-24

### Fixed
- Use the correct `SN_ARCH_ARM64` macro in ARM64 atomics and spinlock (was `SN_ARCH_AARCH64`)

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
