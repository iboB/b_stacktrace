#if !defined(B_STACKTRACE_INCLUDED)
#define B_STACKTRACE_INCLUDED (1)
/*
b_stacktrace v0.10 -- a cross-platform stack-trace generator
URL: https://github.com/iboB/b_stacktrace

Usage
=====

#define B_STACKTRACE_IMPL in *one* C or C++ file to create the implementation

#include "b_stacktrace.h" to get access to the following functions:

* char* b_stacktrace_get();
    Returns a string (composed of multiple lines) which represents the stack trace
    The string is allocated with `malloc` and needs to be freed with `free`

Config
======

#define B_STACKTRACE_API to custom export symbols to export the library functions from a shared lib

Revision History
================

* 0.10 (2020-12-07) Initial public release. Alpha version

MIT License
===========

Copyright (c) 2020 Borislav Stanimirov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#if !defined(B_STACKTRACE_API)
#define B_STACKTRACE_API extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
    Returns a stack-trace string from the point of view of the caller.
    The string is allocated with `malloc` and needs to be freed with `free`
*/
B_STACKTRACE_API char* b_stacktrace_get();

/* version */
#define B_STACKTRACE_VER_MAJOR 0
#define B_STACKTRACE_VER_MINOR 10

#ifdef __cplusplus
}
#endif

#if defined(B_STACKTRACE_IMPL)

#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct print_buf {
    char* buf;
    int pos;
    int size;
} print_buf;

static print_buf buf_init() {
    print_buf ret = {
        (char*) malloc(1024),
        0,
        1024
    };
    return ret;
}

static void buf_printf(print_buf* b, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args) + 1;
    va_end(args);

    const int new_end = b->pos + len;

    if (new_end > b->size) {
        while (new_end > b->size) b->size *= 2;
        b->buf = (char*)realloc(b->buf, b->size);
    }

    va_start(args, fmt);
    b->pos += vsnprintf(b->buf + b->pos, len, fmt, args);
    va_end(args);
}

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

static int SymInitialize_called = 0;

char* b_stacktrace_get() {
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    CONTEXT context;
    STACKFRAME64 frame; /* in/out stackframe */
    DWORD imageType;
    IMAGEHLP_SYMBOL64* symbol = NULL;
    print_buf out = buf_init();

    if (!SymInitialize_called) {
        SymInitialize(process, NULL, TRUE);
        SymInitialize_called = 1;
    }

    RtlCaptureContext(&context);

    memset(&frame, 0, sizeof(frame));
#ifdef _M_IX86
    imageType = IMAGE_FILE_MACHINE_I386;
    frame.AddrPC.Offset = context.Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.Esp;
    frame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
    imageType = IMAGE_FILE_MACHINE_AMD64;
    frame.AddrPC.Offset = context.Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.Rsp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.Rsp;
    frame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
    imageType = IMAGE_FILE_MACHINE_IA64;
    frame.AddrPC.Offset = context.StIIP;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.IntSp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrBStore.Offset = context.RsBSP;
    frame.AddrBStore.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.IntSp;
    frame.AddrStack.Mode = AddrModeFlat;
#else
    #error "Platform not supported!"
#endif

    symbol = (IMAGEHLP_SYMBOL64*)malloc(sizeof(IMAGEHLP_SYMBOL64) + 1024);
    symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
    symbol->MaxNameLength = 1024;

    while (1) {
        IMAGEHLP_LINE64 lineData;
        DWORD lineOffset = 0;
        DWORD64 symOffset = 0;

        if (!StackWalk64(imageType, process, thread, &frame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
            buf_printf(&out, "StackWalk64 error: %d @ %p\n", GetLastError(), frame.AddrPC.Offset);
            break;
        }

        if (frame.AddrPC.Offset == frame.AddrReturn.Offset) {
            buf_printf(&out, "Stack overflow @ %p\n", frame.AddrPC.Offset);
            break;
        }

        SymGetLineFromAddr64(process, frame.AddrPC.Offset, &lineOffset, &lineData);
        buf_printf(&out, "%s(%d): ", lineData.FileName, lineData.LineNumber);

        if (SymGetSymFromAddr64(process, frame.AddrPC.Offset, &symOffset, symbol)) {
            buf_printf(&out, "%s\n", symbol->Name);
        } else {
            buf_printf(&out, " Unkown symbol @ %p\n", frame.AddrPC.Offset);
        }

        if (frame.AddrReturn.Offset == 0) {
            break;
        }
    }

    free(symbol);
    return out.buf;
}

#elif defined(__APPLE__)

#include <execinfo.h>
#include <unistd.h>
#include <dlfcn.h>

char* b_stacktrace_get() {
    void* trace[128];
    int traceSize = backtrace(trace, 128);
    char** messages = backtrace_symbols(trace, traceSize);
    print_buf out = buf_init();
    *out.buf = 0;

    for (int i = 0; i<traceSize; ++i) {
        buf_printf(&out, "%s\n", messages[i]);
    }

    return out.buf;
}

#elif defined(__linux__)

#include <execinfo.h>
#include <ucontext.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

char* b_stacktrace_get() {
    void* trace[1024];
    const int traceSize = backtrace(trace, 1024);
    char** messages = backtrace_symbols(trace, traceSize);
    print_buf out = buf_init();

    for (int i=0; i<traceSize; ++i) {
        char* msg = messages[i];

        /* calculate load offset */
        Dl_info info;
        dladdr(trace[i], &info);
        if (info.dli_fbase == (void*)0x400000) {
            /* address from executable, so don't offset */
            info.dli_fbase = NULL;
        }

        while (*msg && *msg != '(') ++msg;
        *msg = 0;

        {
            char cmd[1024];
            char line[2048];

            FILE* fp;
            snprintf(cmd, 1024, "addr2line -e %s -f -C -p %p 2>/dev/null", messages[i], (void*)((char*)trace[i] - (char*)info.dli_fbase));

            fp = popen(cmd, "r");
            if (!fp) {
                buf_printf(&out, "Failed to generate trace further...\n");
                break;
            }

            while (fgets(line, sizeof(line), fp)) {
                buf_printf(&out, "%s: ", messages[i]);
                if (strstr(line, "?? ")) {
                    /* just output address if nothing can be found */
                    buf_printf(&out, "%p\n", trace[i]);
                } else {
                    buf_printf(&out, "%s", line);
                }
            }

            pclose(fp);
        }
    }

    free(messages);
    return out.buf;
}
#else
/* noop implementation */
char* b_stacktrace_get() {
    print_buf out = buf_init();
    buf_printf("b_stacktrace: unsupported platform\n");
    return out.buf;
}
#endif /* platform */

#endif /* B_STACKTRACE_IMPL */
#endif /* B_STACKTRACE_INCLUDED */

