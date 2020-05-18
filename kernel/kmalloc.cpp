#include <kernel/kstdlib.h>
#include <kernel/kstdio.h>
#include <kernel/pmm.h>
#include <string.h>

void *kmalloc(size_t size) {
	size_t blocks = size/PMMNGR_BLOCK_SIZE + 1;
	if(size % PMMNGR_BLOCK_SIZE == 0) {
		blocks--;
	}

	void* addr = PMM::the->allocBlocks(blocks);
	if((uint32_t) addr == 0) {
		//sout("OUT OF MEMORY\n");
		return 0;
	}
	
	memset(addr, 0x0, blocks * PMMNGR_BLOCK_SIZE);
	sout("KMALLOC: 0x%x - %d block(s) - size: 0x%x\n", addr, blocks, size);
	return addr;
}

void kfree(void *p, size_t size) {
	size_t blocks = size/PMMNGR_BLOCK_SIZE + 1;

	if(size % PMMNGR_BLOCK_SIZE == 0) {
		blocks--;
	}

	sout("KFREE: 0x%x - %d blocks - size: 0x%x\n", uint32_t(p), blocks, size);
	PMM::the->freeBlocks(p, blocks);
}
