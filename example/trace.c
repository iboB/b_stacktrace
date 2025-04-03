/* Copyright (c) Borislav Stanimirov
 * SPDX-License-Identifier: MIT
 */
#define B_STACKTRACE_IMPL
#include <b_stacktrace.h>

void second_level(int n) {
    b_stacktrace_handle trace = b_stacktrace_get();
    char* trace_string = b_stacktrace_to_string(trace);

    printf("Number of arguments to program: %d\n", n);

    printf("Stacktrace (%d entries):\n", b_stacktrace_depth(trace));

    puts(trace_string);
    free(trace_string);
    free(trace);
}

void first_level(int n) {
    second_level(n);
}

int main(int argc, char** argv) {
    first_level(argc);
    return 0;
}
