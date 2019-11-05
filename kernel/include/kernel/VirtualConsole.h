#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include "../../devices/KeyboardDevice.h"
#include <Types.h>
#include <kernel/kstdio.h>

class VirtualConsole : public KeyboardListener {

    public:
        VirtualConsole();

        static VirtualConsole *currentConsole;

        virtual void keyStateChanged(u8 raw, bool pressed) override;

        void putchar(char c);

    private:
        uint16_t* vgaBuffer;
        int row = 0, column = 0;

        const int width = 80, height = 25;
};

#endif