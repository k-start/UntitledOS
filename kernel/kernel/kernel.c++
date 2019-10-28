#include <stdio.h>

#include <kernel/tty.h>

extern "C" {
    void kernel_main(void) {
        terminalInitialize();

        printf("UntitledOS Kernel");
    }
}