#include <kernel/multiboot.h>
#include <kernel/kstdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/CPU.h>
#include <kernel/VirtualConsole.h>
#include <kernel/kstdlib.h>
#include <kernel/devices/KeyboardDevice.h>
#include <kernel/PIT.h>
#include <kernel/RTC.h>

#include <Y/Vector.h>

extern "C" {
    // void kernel_main() {
    void kernel_main(unsigned int ebx, unsigned int magic) {
        multiboot_info_t *mbt = (multiboot_info_t*)(ebx);
        
        mbt->mmap_addr += 0xC0000000;
        multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mbt->mmap_addr;

        gdtInstall();
        CPU::isrInstall();

        // enable interrupts
        asm volatile("sti");

        PIT pit(IRQ0, 100);
        KeyboardDevice keyboard(IRQ1);
        VirtualConsole vConsole;
        RTC rtc(IRQ8);

        printf(" _    _       _   _ _   _          _  ____   _____ \n");
        printf("| |  | |     | | (_) | | |        | |/ __ \\ / ____|\n");
        printf("| |  | |_ __ | |_ _| |_| | ___  __| | |  | | (___  \n");
        printf("| |  | | '_ \\| __| | __| |/ _ \\/ _` | |  | |\\___ \\ \n");
        printf("| |__| | | | | |_| | |_| |  __/ (_| | |__| |____) |\n");
        printf(" \\____/|_| |_|\\__|_|\\__|_|\\___|\\__,_|\\____/|_____/ \n\n");
        
        printf("UntitledOS Kernel\n");
        sout("\n\nUntitledOS Kernel\n");

        // printf("Ram detected: %d kb\n", (mbt->mem_lower + mbt->mem_upper));

        int useableMemory = 0;

        for(int i = 0; i < mbt->mmap_length; i++) {
            if(mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
                sout("%d  addr: %d  len: %d  size: %d\n", i, mmap[i].addr, mmap[i].len, mmap[i].size);
                useableMemory += mmap[i].len;
            }
        }

        printf("Ram detected: %d mb\n", useableMemory/1024/1024);

        // stops main exiting too soon
        for(;;) {
            asm("hlt");
        }
    }
}