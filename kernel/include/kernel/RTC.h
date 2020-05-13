#ifndef RTC_H
#define RTC_H

#include <Types.h>
#include <kernel/IRQHandler.h>

class RTC : public IRQHandler {

    public:
        RTC(u8 IRQNumber);

        virtual void handleIRQ() override;

    private:
        u8 second, minute, hour, day, month, year;

};

#endif