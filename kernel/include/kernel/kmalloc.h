#ifndef KSTDLIB_H
#define KSTDLIB_H

#include <Types.h>
#include <stddef.h>

#define heapStart 0xD0000000

void *kmalloc(size_t size);
void kfree(void *p);

inline void *operator new(size_t size) {
    return kmalloc(size);
}
 
inline void *operator new[](size_t size) {
    return kmalloc(size);
}
 
inline void operator delete(void *p, size_t size) {
    kfree(p);
}
 
inline void operator delete[](void *p, size_t size) {
    kfree(p);
}

inline void operator delete[](void *p) {
    kfree(p);
}


inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };

#endif