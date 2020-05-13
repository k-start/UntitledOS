#ifndef PIT_H
#define PIT_H

#include <Types.h>
#include <kernel/IRQHandler.h>

class PIT : public IRQHandler {

    public:
        PIT(u8 IRQNumber, int freq);

        virtual void handleIRQ() override;

    private:
        int ticks = 0;

};

#endif