#ifndef KSTDLIB_H
#define KSTDLIB_H

#pragma once

#include <Types.h>
#include <stddef.h>

void *kmalloc(size_t size);

inline void *operator new(size_t size)
{
    return kmalloc(size);
}
 
inline void *operator new[](size_t size)
{
    return kmalloc(size);
}
 
inline void operator delete(void *p)
{
    // free(p);
}
 
inline void operator delete[](void *p)
{
    // free(p);
}

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };

#endif