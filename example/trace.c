#define B_STACKTRACE_IMPL
#include "../b_stacktrace.h"

void foo() {
    printf("%s\n", b_stacktrace_get());
}

int main() {
    foo();
}
