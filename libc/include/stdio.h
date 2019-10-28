#ifndef _STDIO_H
#define _STDIO_H 1

#define EOF (-1)

extern "C" {
    int printf(const char* __restrict, ...);
    int putchar(int);
    int puts(const char*);
}

#endif