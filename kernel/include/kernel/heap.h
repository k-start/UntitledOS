#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

class Heap {

    #define heapStart 0xD0000000

    public:

        typedef struct SlabPage {
            SlabPage *next;
            uint32_t startAddr;
            uint32_t endAddr;
        } SlabPage;

        typedef struct Slab {
            size_t allocSize; // the amount of bytes this slab deals with
            SlabPage *firstPage; // pointer to the first page of memory
            SlabPage *lastPage; // pointer to the last page of memory
            size_t slabSize; // amount of pages in the slab
        } Slab;

        Heap();

        void *alloc(size_t size);
        void free(void *p) {}

        static Heap *the;

    private:
        void *slabAlloc(Slab &slab, Slab &allocedSlab);
        void moreMem(Slab &slab);
        SlabPage *newSlabPage();

        uint16_t heapStartOffset = 0;

        Slab slabs[8]; // contains free memory
        Slab allocedSlabs[8]; // contains alloced memory
        size_t slabCount = 8;
};

#endif