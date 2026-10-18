# Windows x64 Anti-Debugging PoC

A lightweight C++ Proof of Concept demonstrating low-level user-mode anti-analysis and anti-debugging techniques on modern Windows x64. 

This project explores mechanisms designed to detect dynamic reverse engineering environments without relying on standard high-level WinAPI calls (such as `IsDebuggerPresent`), which are trivial to hook and bypass.

## Implemented Techniques

1. **CPU Timing Analysis (`RDTSC`):**
   - Utilizes the `__rdtsc()` compiler intrinsic to measure processor cycle deltas during a baseline operation.
   - Detects execution pauses and single-stepping introduced by dynamic debuggers.

2. **Hardware Breakpoint Detection (`Dr0-Dr3`):**
   - Retrieves the current thread execution context via `GetThreadContext()` with `CONTEXT_DEBUG_REGISTERS`.
   - Inspects debug registers `Dr0`, `Dr1`, `Dr2`, and `Dr3` for active memory/execution breakpoints set by an analyst.

3. **Direct PEB & `NtGlobalFlag` Inspection:**
   - Obtains a direct pointer to the Process Environment Block (PEB) using `__readgsqword(0x60)` via the `GS` segment register.
   - Bypasses user-mode API hooks by directly inspecting:
     - `BeingDebugged` flag (offset `0x02`).
     - `NtGlobalFlag` (offset `0xBC`) with the bitmask `0x70` (`FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_FREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS`).

## Build & Test

- **Platform:** Windows 10/11 x64
- **Toolchain:** MSVC (Visual Studio 2022 / C++20 standard)
- **Configuration:** `Release | x64`

```cmd
# Normal execution:
Prints "HACKED"

# Debugger execution (x64dbg / Visual Studio debugger):
Process silently terminates (exit code 0) prior to execution of core payload logic.
