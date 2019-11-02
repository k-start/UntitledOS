#ifndef IRQHANDLER_H
#define IRQHANDLER_H

#include <Types.h>
#include <kernel/isr.h>

class IRQHandler {

    public:
        IRQHandler(u8 IRQNumber);

        virtual void handleIRQ() { };
        u8 getIRQNumber();

    private:
        u8 IRQNumber;
};

#endif