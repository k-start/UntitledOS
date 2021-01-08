#include <kernel/multiboot.h>
#include <kernel/kstdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/CPU.h>
#include <kernel/VirtualConsole.h>
#include <kernel/kmalloc.h>
#include <kernel/devices/KeyboardDevice.h>
#include <kernel/PIT.h>
#include <kernel/RTC.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>
#include <kernel/Filesystem/VFS.h>
#include <kernel/Filesystem/IDE.h>

extern "C" {

    // extern uint32_t PageDirectoryVirtualAddress;
    
    void kernel_main(unsigned int ebx, unsigned int magic) {
        sout("kernel %x\n", ebx);
        multiboot_info_t *mbt = (multiboot_info_t*)(ebx);
        
         // initialize memory managers
        PMM pmm(mbt); // physical
        VMM vmm(&pmm); // virtual
        Heap heap; // kernel heap

        IDE ide;

        // initialise the virtual filesystem
        VFS vfs;

        gdtInstall(); // FIX ME - move to CPU
        CPU::isrInstall();

        // enable interrupts
        asm volatile("sti");

        PIT pit(IRQ0, 100);
        KeyboardDevice keyboard(IRQ1);
        RTC rtc(IRQ8);

        VirtualConsole vConsole;

        printf(" _    _       _   _ _   _          _  ____   _____ \n");
        printf("| |  | |     | | (_) | | |        | |/ __ \\ / ____|\n");
        printf("| |  | |_ __ | |_ _| |_| | ___  __| | |  | | (___  \n");
        printf("| |  | | '_ \\| __| | __| |/ _ \\/ _` | |  | |\\___ \\ \n");
        printf("| |__| | | | | |_| | |_| |  __/ (_| | |__| |____) |\n");
        printf(" \\____/|_| |_|\\__|_|\\__|_|\\___|\\__,_|\\____/|_____/ \n\n");
        
        printf("UntitledOS Kernel\n");
        sout("UntitledOS Kernel\n");

        printf("Ram detected: %d MB\n", (mbt->mem_lower + mbt->mem_upper)/1024 + 1);

        vConsole.newCommand();

        // stops main exiting too soon
        for(;;) {
            // asm("hlt");
        }
    }
}