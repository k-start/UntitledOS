#ifndef PIT_H
#define PIT_H

#include <Types.h>
#include <kernel/IRQHandler.h>
#include <Y/String.h>

class PIT : public IRQHandler {

    public:
        PIT(u8 IRQNumber, int freq);

        virtual void handleIRQ() override;

        String getUptimeStr();

        static PIT *the;

        void sleep(int ms);

    private:
        int ticks = 0;
        int seconds = 0;

        bool sleeping = false;
        int sleepMS = 0;

};

#endif