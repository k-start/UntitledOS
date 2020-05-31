#include <kernel/heap.h>
#include <string.h>
#include <kernel/kstdio.h>
#include <kernel/vmm.h>
#include <kernel/pmm.h>

Heap *Heap::the;

Heap::Heap() {
    the = this;

    // create our slabs
    size_t size = 32;
    for(size_t i = 0; i < slabCount; i++) {
        slabs[i].allocSize = size;
        slabs[i].firstFrame = NULL;
        slabs[i].slabSize = 0;

        allocedSlabs[i].allocSize = size;
        allocedSlabs[i].firstFrame = NULL;
        allocedSlabs[i].slabSize = 0;

        size = size * 2;
    }
}

// FIX ME - potentially adding a smaller slab size and removing the 2048+ slab to just allocate pages
void *Heap::alloc(size_t size) {
    if(size > 4096) {
        sout("CANT ALLOC SIZE MORE THAN 1 PAGE\n");
        return 0;
    }

    for(size_t i; i < slabCount; i++) {
        if(size <= slabs[i].allocSize) {
            // Allocate out of this slab
            return slabAlloc(slabs[i], allocedSlabs[i]);
        }
    }

    return 0;
}

// free with unknown amount of memory
void Heap::free(void *p) {
    uint32_t address = (uint32_t)p;

    // we dont know how big the pointer is so we need to search all the allocced slabs
    for(size_t i; i < slabCount; i++) {
        Heap::Slab *slab = &allocedSlabs[i];
        Heap::SlabFrame *frame = slab->firstFrame;
        bool found = false;

        // loop through all frames to see if we can find it
        while (frame != NULL) {
            // we found it!
            if(frame->startAddr == address) {
                found = true;
                // move the frame into the free slab
                Heap::SlabFrame *nextAllocedFrame = frame->next;
                frame->next = slabs[i].firstFrame;
                slabs[i].firstFrame = frame;

                // remove it from the alloced slab
                slab->firstFrame = nextAllocedFrame;
                break;
            }
            frame = frame->next;
        }

        // if we found the address no need to continue searching
        if(found) {
            break;
        }
    }
}

// Fix me - if/else flow
// allocates a frame of memory out of the slab
void *Heap::slabAlloc(Heap::Slab &slab, Heap::Slab &allocedSlab) {
    if(slab.firstFrame == NULL) {
        moreMem(slab);
    }

    Heap::SlabFrame *firstFrame = slab.firstFrame;

    // if we have more memory in the page than a single allocation create a new page for the alloced slabs and readjust
    // if not we just move the current page into the alloced slabs
    if(firstFrame->endAddr - firstFrame->startAddr > slab.allocSize) {
        Heap::SlabFrame *allocedFrame = newSlabFrame();
        allocedFrame->startAddr = firstFrame->startAddr;
        allocedFrame->endAddr = firstFrame->startAddr + slab.allocSize - 1;
        allocedFrame->next = NULL;

        // resize the frame in the free slab
        firstFrame->startAddr = firstFrame->startAddr + slab.allocSize;

        // add the allocated frame into the allocated slab
        allocedFrame->next = allocedSlab.firstFrame;
        allocedSlab.firstFrame = allocedFrame;

        return (void*)allocedFrame->startAddr;
    } else {
        // move the slab firstFrame to the next if it exists
        slab.firstFrame = firstFrame->next;

        // add the allocated frame into the allocated slab
        firstFrame->next = allocedSlab.firstFrame;
        allocedSlab.firstFrame = firstFrame;

        return (void*)firstFrame->startAddr;
    }
}

// gets another page of memory for the slab
void Heap::moreMem(Heap::Slab &slab) {
    sout("MORE MEM: %d\n", slab.allocSize);
    if(slab.firstFrame != NULL) {
        // If the first frame isnt null then it means we already have memory
        // Why are we here?
        sout("Trying to allocate memory with an existing frame\n");
        return;
    }

    // get a new frame to put memory in
    slab.firstFrame = newSlabFrame();
    // get a page from of virtual memory, move the heapStartOffest by 1 page
    uint32_t memory = (uint32_t)VMM::the->allocPage(heapStart + heapStartOffset);
    if(memory != 0) {
        heapStartOffset += 0x1000;
    }

    // fill the frame
    slab.firstFrame->startAddr = memory;
    slab.firstFrame->endAddr = memory + 0x1000 - 1;
}

// returns an empty slab frame
Heap::SlabFrame *Heap::newSlabFrame() {
    Heap::SlabFrame *slabFrame = (Heap::SlabFrame*)PMM::kmem(sizeof(Heap::SlabFrame));
    slabFrame->startAddr = 0;
    slabFrame->endAddr = 0;
    slabFrame->next = NULL;
    return slabFrame;
}