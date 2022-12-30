# b_stacktrace

A minimalistic single-header multi-platform C89 lib for stack tracing

## Usage

### C or C++

Clone the repo or copy `b_stacktrace.h` in your project tree.

Including `b_stacktrace.h` provides:

* `char* b_stacktrace_get_string();`- Returns a human-readable stack-trace string from the point of view of the caller. The string is allocated with `malloc` and needs to be freed with `free`.
* `b_stacktrace_handle b_stacktrace_get();` - Returns a stack-trace handle from the point of view of the caller which can be expanded to a string via `b_stacktrace_to_string`. The handle is allocated with `malloc` and needs to be freed with `free`.
* `b_stacktrace_to_string(b_stacktrace_handle stacktrace);` - Converts a stack-trace handle to a human-readable string. The string is allocated with `malloc` and needs to be freed with `free`.

`#define B_STACKTRACE_IMPL` before including `b_stacktrace.h` in *one* C or C++ file to create the implementation

### Optional wrapper for C++17 and newer

*TBD*

## Examples

The subdirectory `example/` contains several small examples:

* `trace.c` - creates a trace from a C program
* `trace.cpp` - creates a trace from a C++ program, demonstrating more complex symbols (classes, templates)
* `crash.c` - example usage of the provided stack trace: as a crash handler

## License

The library is distributed under the [MIT Software License](http://opensource.org/licenses/MIT). Copyright &copy; 2020-2022 Borislav Stanimirov.
