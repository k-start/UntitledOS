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
#include <kernel/pmm.h>

#include <Y/Vector.h>

extern "C" {
    
    void kernel_main(unsigned int ebx, unsigned int magic) {
        multiboot_info_t *mbt = (multiboot_info_t*)(ebx);
        
        PMM pmm(mbt); // initialize physical memory manager

        gdtInstall();
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

        printf("Ram detected: %d mb\n", (mbt->mem_lower + mbt->mem_upper)/1024);

        new Vector<int>;

        // stops main exiting too soon
        for(;;) {
            asm("hlt");
        }
    }
}