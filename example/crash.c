/* Copyright (c) Borislav Stanimirov
 * SPDX-License-Identifier: MIT
 */
#define B_STACKTRACE_IMPL
#include "../b_stacktrace.h"
#include <signal.h>
#include <stdint.h>

#if defined(_WIN32)
void my_handler(int signal) {
    printf("signal: %d\n", signal);
    puts(b_stacktrace_get_string());
    exit(1);
}
void register_handler() {
    signal(SIGSEGV, my_handler);
    signal(SIGABRT, my_handler);
}
#else
void my_handler(int signal, siginfo_t* info, void* context) {
    context; // unused
    printf("signal: %d", signal);
    if (signal == SIGSEGV) {
        printf(" at addr: %p", info->si_addr);
    }
    puts("");
    puts(b_stacktrace_get_string());
    exit(1);
}
void register_handler() {
    struct sigaction sa;
    sa.sa_sigaction = my_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
}
#endif

int crasher(int a) {
    char *p = (char *)((uintptr_t)0xdeadbeef);
    *p = 10;  /* CRASH here!! */
    return 2 * a + *p;
}

int func(int n) {
    return 5 + crasher(n);
}

int main(int argc, char** argv) {
    register_handler();
    return func(argc);
}
