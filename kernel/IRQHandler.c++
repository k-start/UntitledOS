#include "IRQHandler.h"

IRQHandler::IRQHandler(u8 IRQNumber) : IRQNumber(IRQNumber) {
    CPU::setInterruptHandler(IRQNumber, this);
}

u8 IRQHandler::getIRQNumber() {
    return IRQNumber;
}