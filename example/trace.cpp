#define B_STACKTRACE_IMPL
#include "../b_stacktrace.h"

#include <iostream>

struct base
{
    virtual ~base() {}

    void print_trace() {
        std::cout << b_stacktrace_get() << '\n';
    }

    virtual void proxy() = 0;
};

struct impl : base
{
    virtual void proxy() {
        print_trace();
    }
};

template <typename T>
void trampoline(T& t)
{
    t.proxy();
}

int main() {
    impl i;
    trampoline(i);
}

