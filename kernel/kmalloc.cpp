#include <kernel/kstdlib.h>
#include <kernel/kstdio.h>

const size_t totalSize = 3 * 1024 * 1024 / 32 / 8;

static u8 allocMap[totalSize];
static size_t offset = 0;

void *kmalloc(size_t size) {
	offset += size;
	if(offset >= totalSize) {
		kprintf("Kernel out of memory\n");
		asm("hlt");
		return 0;
	}
	return allocMap + offset - size;
}