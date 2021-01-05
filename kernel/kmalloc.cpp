#include <kernel/kmalloc.h>
#include <kernel/kstdio.h>
// #include <kernel/pmm.h>
// #include <kernel/vmm.h>
#include <kernel/heap.h>
#include <string.h>

int alloced = 0;

void *kmalloc(size_t size) {
	// size_t blocks = size/BLOCK_SIZE + 1;
	// if(size % BLOCK_SIZE == 0) {
	// 	blocks--;
	// }

	// // void* addr = PMM::the->allocBlocks(blocks);
	// void *addr = VMM::the->allocPages(blocks, heapStart + (alloced * 0x1000));
	// alloced++;
	// if((uint32_t) addr == 0) {
	// 	//sout("OUT OF MEMORY\n");
	// 	return 0;
	// }
	
	// memset(addr, 0x0, blocks * PMMNGR_BLOCK_SIZE);
	// sout("KMALLOC: 0x%x - %d block(s) - size: 0x%x\n", addr, blocks, size);

	void *addr = Heap::the->alloc(size);
	// sout("KMALLOC: 0x%x - size: 0x%x\n", addr, size);

	return addr;
}

void kfree(void *p) {
	// size_t blocks = size/BLOCK_SIZE + 1;

	// if(size % BLOCK_SIZE == 0) {
	// 	blocks--;
	// }

	// sout("KFREE: 0x%x - %d blocks - size: 0x%x\n", uint32_t(p), blocks, size);
	// PMM::the->freeBlocks(p, blocks);
	// sout("KFREE: 0x%x\n", uint32_t(p));
	Heap::the->free(p);
}