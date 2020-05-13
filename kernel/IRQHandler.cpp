#include <kernel/IRQHandler.h>
#include <kernel/CPU.h>

IRQHandler::IRQHandler(u8 IRQNumber) : IRQNumber(IRQNumber) {
    CPU::setInterruptHandler(IRQNumber, this);
}

u8 IRQHandler::getIRQNumber() {
    return IRQNumber;
}