#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stddef.h>

extern "C" {
    __attribute__((__noreturn__))
    void abort(void);

    void itoa(int n, char str[]);

    void *malloc(size_t size);
}

#endif