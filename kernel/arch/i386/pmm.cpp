#include <kernel/pmm.h>
#include <stdio.h>
#include <string.h>
#include <kernel/kstdio.h>

PMM *PMM::the;
uint32_t PMM::memorySize;
uint32_t PMM::usedBlocks;
uint32_t PMM::maxBlocks;
uint32_t* PMM::memoryMap;

extern "C" void* _kernel_start;
extern "C" void* _kernel_start_physical;
extern "C" void* _kernel_end;
extern "C" void* _kernel_end_physical;
extern "C" void* _kernel_size;
extern "C" uint32_t PageDirectoryVirtualAddress[1024];

char* strMemoryTypes[] = {
	"Available",		//type==1
	"Reserved",			//type==2
	"ACPI Reclaim",		//type==3
	"ACPI NVS Memory",	//type==4
    "BAD RAM"           //type==5
};

PMM::PMM(multiboot_info_t *mbt) {
    the = this;
    this->mbt = mbt;

    int useableMemory = mbt->mem_lower + mbt->mem_upper;

    memorySize = useableMemory;
    usedBlocks = (useableMemory * 1024) / PMMNGR_BLOCK_SIZE;
    maxBlocks = (useableMemory * 1024) / PMMNGR_BLOCK_SIZE;
    memoryMap = (uint32_t*)(&_kernel_end);

    memset (memoryMap, 0xf, maxBlocks / PMMNGR_BLOCKS_PER_BYTE );

    mbt->mmap_addr += 0xC0000000;
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)mbt->mmap_addr;

    sout("**************** MEMORY MAP ****************\n");
    for(int i = 0; i < mbt->mmap_length; i++) {
        int memType = mmap[i].type - 1;
        if(memType >= 0 && memType <= 4) {
            sout("%d  addr: 0x%x  len: 0x%x  size: 0x%x  ", i, mmap[i].addr, mmap[i].len, mmap[i].size);
            sout("%s\n", strMemoryTypes[memType]);

            if(memType + 1 == MULTIBOOT_MEMORY_AVAILABLE) {
                initRegion(mmap[i].addr, mmap[i].len);
            }
        }
    }
    sout("********************************************\n");

    // deinitRegion(0x100000, size_t(&_kernel_size));
    // deinitRegion(0x0, 0x9FC00);
    deinitRegion(int(&_kernel_start_physical), int(&_kernel_size) + 0x1000);

    sout("Kernel start: 0x%x\n", &_kernel_start);
    sout("Kernel physical start: 0x%x\n", &_kernel_start_physical);
    sout("Kernel end: 0x%x\n", &_kernel_end);
    sout("Kernel physical end: 0x%x\n", &_kernel_end_physical);
    sout("Kernel size: 0x%x\n", &_kernel_size);
    sout("Ram detected: %d mb\n", (useableMemory/1024 + 1));
    sout("Max blocks: %d  memoryMap size: 0x%x\n", maxBlocks, maxBlocks / PMMNGR_BLOCKS_PER_BYTE);

    sout("Available Blocks: %d\n", maxBlocks - usedBlocks);

	uint32_t address = (uint32_t)(0x0);
	address |= 1 << 7;
	address |= 1 << 1;
	address |= 1 << 0;
	PageDirectoryVirtualAddress[(0x0 >> 22)] = address;

	// address = (uint32_t)(0x400000);
	// address |= 1 << 7;
	// address |= 1 << 1;
	// address |= 1 << 0;
	// PageDirectoryVirtualAddress[(0x400000 >> 22)] = address;
}

void PMM::mmapSet(int bit) {
    memoryMap[bit / 32] |= (1 << (bit % 32));
}

void PMM::mmapUnset(int bit) {
    memoryMap[bit / 32] &= ~ (1 << (bit % 32));
}

bool PMM::mmapTest(int bit) {
    return memoryMap[bit / 32] &  (1 << (bit % 32));
}

int PMM::mmapFirstFree() {
	for (uint32_t i = 0; i < maxBlocks / 32; i++) {
		if (memoryMap[i] != 0xffffffff) {
			for (int j = 0; j < 32; j++) {
				int bit = 1 << j;
				if (!(memoryMap[i] & bit)) {
					return i*4*8+j;
                }
			}
        }
    }
	return -1;
}

int PMM::mmapFirstFreeS(size_t size) {
	if (size==0) {
		return -1;
    }

	if (size==1) {
		return mmapFirstFree();
    }

	for (uint32_t i = 0; i < maxBlocks / 32; i++)
		if (memoryMap[i] != 0xffffffff)
			for (int j = 0; j < 32; j++) {	//! test each bit in the dword

				int bit = 1<<j;
				if (!(memoryMap[i] & bit)) {

					int startingBit = i*32;
					startingBit += bit;		//get the free bit in the dword at index i

					uint32_t free = 0; //loop through each bit to see if its enough space
					for (uint32_t count = 0; count <= size;count++) {

						if (!mmapTest(startingBit+count)) {
							free++;	// this bit is clear (free frame)
                        }

						if(free==size) {
							return i*4*8+j; //free count==size needed; return index
                        }
					}
				}
			}

	return -1;
}

void PMM::initRegion(uint32_t base, size_t size) {
	int align = base / PMMNGR_BLOCK_SIZE;
	int blocks = size / PMMNGR_BLOCK_SIZE;
 
	for (; blocks > 0; blocks--) {
		mmapUnset(align++);
		usedBlocks--;
	}
 
	mmapSet(0);	//first block is always set. This insures allocs cant be 0
}

void PMM::deinitRegion(uint32_t base, size_t size) {
	int align = base / PMMNGR_BLOCK_SIZE;
	int blocks = size / PMMNGR_BLOCK_SIZE;
 
	for (; blocks > 0; blocks--) {
		mmapSet(align++);
		usedBlocks++;
	}
}

void* PMM::allocBlock() {
	if (maxBlocks - usedBlocks <= 0) {
        sout("OUT OF MEMORY\n");
		return 0;	//out of memory
    }
 
	int frame = mmapFirstFree();
 
	if (frame == -1) {
        sout("OUT OF MEMORY - FRAME\n");
		return 0;	//out of memory
    }

	mmapSet(frame);

	uint32_t addr = frame * PMMNGR_BLOCK_SIZE;
	usedBlocks++;
 
	return (void*)addr;
}

void* PMM::allocBlocks(size_t size) {
	if (maxBlocks - usedBlocks <= size) {
        sout("OUT OF MEMORY\n");
		return 0;	//not enough space
    }

	int frame = mmapFirstFreeS(size);

	if (frame == -1) {
        sout("OUT OF MEMORY - FRAME\n");
		return 0;	//not enough space
    }
    
	for (uint32_t i=0; i<size; i++) {
		mmapSet(frame+i);
    }
    
	uint32_t addr = frame * PMMNGR_BLOCK_SIZE;
	usedBlocks += size;

	return (void*)addr;
}

void PMM::freeBlock(void* p) {
	uint32_t addr = (uint32_t)p;
	int frame = addr / PMMNGR_BLOCK_SIZE;
 
	mmapUnset(frame);
 
	usedBlocks--;
}

void PMM::freeBlocks(void* p, size_t size) {
	uint32_t addr = (uint32_t)p;
	int frame = addr / PMMNGR_BLOCK_SIZE;

	for (uint32_t i = 0; i < size; i++) {
		mmapUnset(frame+i);
	}

	usedBlocks -= size;
}