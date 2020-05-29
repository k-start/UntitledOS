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
        slabs[i].firstPage = NULL;
        slabs[i].lastPage = NULL;
        slabs[i].slabSize = 0;

        allocedSlabs[i].allocSize = size;
        allocedSlabs[i].firstPage = NULL;
        allocedSlabs[i].lastPage = NULL;
        allocedSlabs[i].slabSize = 0;

        size = size * 2;
    }
}

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

void *Heap::slabAlloc(Heap::Slab &slab, Heap::Slab &allocedSlab) {
    if(slab.firstPage == NULL) {
        moreMem(slab);
    }

    Heap::SlabPage *firstPage = slab.firstPage;

    // if we have more memory in the page than a single allocation create a new page for the alloced slabs and readjust
    // if not we just move the current page into the alloced slabs
    if(firstPage->endAddr - firstPage->startAddr > slab.allocSize) {
        sout("Lota space\n");
        Heap::SlabPage *allocedPage = newSlabPage();
        allocedPage->startAddr = firstPage->startAddr;
        allocedPage->endAddr = firstPage->startAddr + slab.allocSize - 1;
        allocedPage->next = NULL;

        firstPage->startAddr = firstPage->startAddr + slab.allocSize;

        if(allocedSlab.lastPage == NULL) {
            allocedSlab.firstPage = allocedPage;
            allocedSlab.lastPage = allocedPage;
        } else {
            allocedSlab.lastPage->next = allocedPage;
        }

        return (void*)allocedPage->startAddr;
    } else {
        sout("last page!\n");
        if(allocedSlab.lastPage == NULL) {
            allocedSlab.firstPage = firstPage;
            allocedSlab.lastPage = firstPage;
        } else {
            allocedSlab.lastPage->next = firstPage;
        }

        if(firstPage->next == NULL) {
            slab.firstPage = NULL;
            slab.lastPage = NULL;
        } else {
            slab.firstPage = firstPage->next;
        }

        return (void*)firstPage->startAddr;
    }
}

void Heap::moreMem(Heap::Slab &slab) {
    sout("MORE MEM: %d\n", slab.allocSize);
    if(slab.lastPage == NULL) {
        slab.lastPage = newSlabPage();
        slab.firstPage = slab.lastPage;
    }

    uint32_t memory = (uint32_t)VMM::the->allocPage(heapStart + heapStartOffset);
    if(memory != 0) {
        heapStartOffset += 0x1000;
    }

    // last page not empty
    if(slab.lastPage->startAddr != 0) {
        slab.lastPage->next = newSlabPage();
        slab.lastPage = slab.lastPage->next;
    }
    slab.lastPage->startAddr = memory;
    slab.lastPage->endAddr = memory + 0x1000 - 1;
}

Heap::SlabPage *Heap::newSlabPage() {
    Heap::SlabPage *slabPage = (Heap::SlabPage*)PMM::kmem(sizeof(Heap::SlabPage));
    slabPage->startAddr = 0;
    slabPage->endAddr = 0;
    slabPage->next = NULL;
    return slabPage;
}