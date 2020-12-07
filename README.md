# b_stacktrace

A minimalistic single-header multi-platform C89 lib for stack tracing

## Usage

Clone the repo or copy `b_stacktrace.h` in your project tree.

Including `b_stacktrace.h` provides:

`char* b_stacktrace_get();`

It returns a string (composed of multiple lines) which represents the stack trace. The string is allocated with `malloc` and needs to be freed with `free`.

`#define B_STACKTRACE_IMPL` before including `b_stacktrace.h` in *one* C or C++ file to create the implementation

## Examples

The subdirectory `example/` contains several small examples:

* `trace.c` - creates a trace from a C program
* `trace.cpp` - creates a trace from a C++ program, demonstrating more complex symbols (classes, templates)
* `crash.c` - example usage of the provided stack trace: as a crash handler

## License

The library is distributed under the [MIT Software License](http://opensource.org/licenses/MIT). Copyright &copy; 2020 Borislav Stanimirov.
