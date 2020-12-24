#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

class Heap {

    #define heapStart 0xD0000000

    public:

        typedef struct SlabFrame {
            SlabFrame *next;
            uint32_t startAddr;
            uint32_t endAddr;
        } SlabFrame;

        typedef struct Slab {
            size_t allocSize;      // the amount of bytes this slab deals with
            SlabFrame *firstFrame; // pointer to the first frame of memory
            size_t slabSize;       // amount of frames in the slab
        } Slab;

        Heap();

        void *alloc(size_t size);
        void free(void *p);

        static Heap *the;

    private:
        void *slabAlloc(Slab &slab, Slab &allocedSlab);
        void moreMem(Slab &slab);
        SlabFrame *newSlabFrame();

        uint16_t heapStartOffset = 0;

        Slab slabs[8]; // contains free memory
        Slab allocedSlabs[8]; // contains alloced memory
        size_t slabCount = 8;
};

#endif