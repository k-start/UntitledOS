// #include <stdlib.h>
// #if defined(__is_libk)
// #include <kernel/kmalloc.h>
// #endif

// void *malloc(size_t size) {
// #if defined(__is_libk)
// 	return kmalloc(size);
// #else
// 	return 0;
// #endif
// }