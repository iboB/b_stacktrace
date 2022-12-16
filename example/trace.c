#define B_STACKTRACE_IMPL
#include "../b_stacktrace.h"

void second_level(int n) {
    char* trace = b_stacktrace_get_string();

    printf("Number of arguments to program: %d\n", n);

    puts("Stacktrace:\n");
    puts(trace);
    free(trace);
}

void first_level(int n) {
    second_level(n);
}

int main(int argc, char** argv) {
    first_level(argc);
}
