#ifndef RTC_H
#define RTC_H

#include <Types.h>
#include <kernel/IRQHandler.h>
#include <Y/String.h>

class RTC : public IRQHandler {

    public:
        RTC(u8 IRQNumber);

        virtual void handleIRQ() override;

        String getTime();
        String getDate();

        static RTC *the;

    private:
        u8 second, minute, hour, day, month, year;

};

#endif