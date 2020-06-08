#ifndef FDC_H
#define FDC_H

#include <kernel/IRQHandler.h>
#include <Types.h>
#include <stdint.h>
#include <stddef.h>

class FDC : public IRQHandler {

    public:

        FDC(u8 IRQNumber);

        virtual void handleIRQ() override;

        static FDC *the;

    private:
        

};

#endif