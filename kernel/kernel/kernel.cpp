#include <kernel/kstdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/CPU.h>
#include <kernel/VirtualConsole.h>
#include <kernel/kstdlib.h>

#include <Y/Vector.h>

#include <kernel/devices/KeyboardDevice.h>
#include <kernel/PIT.h>
#include <kernel/RTC.h>

extern "C" {

    void kernel_main(void) {
        gdtInstall();
        CPU::isrInstall();

        // enable interrupts
        asm volatile("sti");

        PIT pit(IRQ0, 100);
        KeyboardDevice keyboard(IRQ1);
        VirtualConsole vConsole;
        RTC rtc(IRQ8);

        kprintf(" _    _       _   _ _   _          _  ____   _____ \n");
        kprintf("| |  | |     | | (_) | | |        | |/ __ \\ / ____|\n");
        kprintf("| |  | |_ __ | |_ _| |_| | ___  __| | |  | | (___  \n");
        kprintf("| |  | | '_ \\| __| | __| |/ _ \\/ _` | |  | |\\___ \\ \n");
        kprintf("| |__| | | | | |_| | |_| |  __/ (_| | |__| |____) |\n");
        kprintf(" \\____/|_| |_|\\__|_|\\__|_|\\___|\\__,_|\\____/|_____/ \n\n");
        
        kprintf("UntitledOS Kernel\n");

        // stops main exiting too soon
        for(;;) {
            asm("hlt");
        }
    }
}