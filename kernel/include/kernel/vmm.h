#ifndef VMM_H
#define VMM_H

#include <stdint.h>

#define PAGE_DIR_ADDR           0xFFFFF000
#define PAGE_TABLE_BASE_ADDR    0xFFC00000

#define PAGE_DIRECTORY_INDEX(x) ((x) >> 22)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)

#define PAGE_SIZE 4096

class PMM;

class VMM {

    public:
        typedef uint32_t pDirectory;
        typedef uint32_t pDirEntry;
        typedef uint32_t pTableEntry;
        typedef uint32_t pTable;
        enum PAGE_PTE_FLAGS {
            PTE_PRESENT	      = 1,
            PTE_WRITABLE      = 2,
            PTE_USER          = 4,
            PTE_WRITETHOUGH	  = 8,
            PTE_NOT_CACHEABLE = 0x10,
            PTE_ACCESSED      = 0x20,
            PTE_DIRTY	      = 0x40,
            PTE_PAT		      = 0x80,
            PTE_CPU_GLOBAL    = 0x100,
            PTE_LV4_GLOBAL    = 0x200,
        };
        enum PAGE_PDE_FLAGS {
            PDE_PRESENT    = 1,
            PDE_WRITABLE   = 2,
            PDE_USER       = 4,
            PDE_PWT        = 8,
            PDE_PCD        = 0x10,
            PDE_ACCESSED   = 0x20,
            PDE_DIRTY      = 0x40,
            PDE_4MB        = 0x80,
            PDE_CPU_GLOBAL = 0x100,
            PDE_LV4_GLOBAL = 0x200,
        };

        VMM(PMM *pmm);

        void *allocPage(uint32_t address);
        void *allocPages(unsigned int pages, uint32_t address);
        void mapPage(void *phys, void *virt);

        static VMM *the;

    private:
        PMM *pmm;

};

#endif