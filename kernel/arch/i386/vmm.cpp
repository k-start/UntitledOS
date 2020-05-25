#include <kernel/vmm.h>
#include <kernel/pmm.h>
#include <string.h>
#include <kernel/kstdio.h>

VMM *VMM::the;

extern "C" uint32_t BootPageDirectory[1024];

VMM::VMM(PMM *pmm) {
    the = this;
    this->pmm = pmm;
}

void VMM::mapPage(void *phys, void *virt) {
    pDirectory *pageDirectory = (pDirectory*) PAGE_DIR_ADDR; // mapped to address 0xFFFFF000

    uint32_t pageDirIndex = PAGE_DIRECTORY_INDEX((uint32_t) virt);
    uint32_t pageTableIndex = PAGE_TABLE_INDEX((uint32_t) virt);

    pDirEntry *pde = &pageDirectory[pageDirIndex];
    // if there is no page table available we need to make one - get a block of memory and map it
    if((*pde & PDE_PRESENT) != PDE_PRESENT) {
        uint32_t tableAddr = (uint32_t)pmm->allocBlock();
        if(!tableAddr) { // out of memory
            return;
        }
        tableAddr |= PDE_PRESENT;
        tableAddr |= PDE_WRITABLE;
        pageDirectory[pageDirIndex] = tableAddr;

        // Get the table mapped from virtual memory thanks to recursive mapping
        pTable *table = ((uint32_t*) PAGE_TABLE_BASE_ADDR) + (0x400 * pageDirIndex); // 0xFFC00000 + 0x400 * index.
        // clear the table
        memset(table, 0, sizeof(table));
    }
    // We must have a valid page directory entry to be here

    // Get the table mapped from virtual memory thanks to recursive mapping
    pTable *table = ((uint32_t*) PAGE_TABLE_BASE_ADDR) + (0x400 * pageDirIndex); // 0xFFC00000 + 0x400 * index.
    pTableEntry *pte = &table[pageTableIndex];

    if((*pte & PTE_PRESENT) == PTE_PRESENT) {
        // Means the page must already be allocated to some physical memory
        // FIX ME implement this error check
        sout("VIRTUAL ADDRESS ALREADY MAPPED\n");
        return;
    }

    uint32_t pageAddr = (uint32_t)phys;
    pageAddr |= PTE_PRESENT;
    pageAddr |= PTE_WRITABLE;

    table[pageTableIndex] = pageAddr;
}