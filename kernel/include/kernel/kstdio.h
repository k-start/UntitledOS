#ifndef KSTDIO_H
#define KSTDIO_H

#define EOF (-1)

int kprintf(const char* __restrict, ...);
int kputchar(int);

int sout(const char* __restrict, ...);
int soutchar(int);

#endif