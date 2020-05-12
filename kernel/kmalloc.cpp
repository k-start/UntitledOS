#include <kernel/kstdlib.h>

static u8 allocMap[(3 >> 20) / 32 / 8];

void *kmalloc(size_t size) {
	return allocMap;
}