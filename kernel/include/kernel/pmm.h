#ifndef PMM_H
#define PMM_H

#include <kernel/multiboot.h>
#include <stdint.h>
#include <stddef.h>

#define BLOCKS_PER_BYTE 8
#define BLOCK_SIZE 4096
#define BLOCK_ALIGN BLOCK_SIZE

class PMM {

    public:
        PMM(multiboot_info_t *mbt);

        void mmapSet(int bit);
        void mmapUnset (int bit);
        bool mmapTest(int bit);
        int mmapFirstFree();
        int mmapFirstFreeS(size_t size);
        void initRegion(uint32_t base, size_t size);
        void deinitRegion(uint32_t base, size_t size);
        void* allocBlock();
        void* allocBlocks(size_t size);
        void freeBlock(void* p);
        void freeBlocks(void* p, size_t size);


        // fix me - move elsewhere
        static void *kmem(size_t size) {
            void *addr = (void*)kernelEnd;
            kernelEnd += size;
            return addr;
        }

        static PMM *the;
        static uint32_t memorySize;
        static uint32_t usedBlocks;
        static uint32_t maxBlocks;
        static uint32_t* memoryMap;

    private:
        multiboot_info_t *mbt;
        static uint32_t kernelEnd;

};

#endif