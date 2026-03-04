#include "chips.hpp"
#include <iostream>

Chip4017::Chip4017(uint8_t Qreset) {
    if (Qreset < 1 || Qreset > 10) {
        throw std::invalid_argument("Qreset precisa estar entre 1 e 10");
    }
    this->Qreset = Qreset;
    updateOutputs();
}

void Chip4017::setClockEnable(bool enable) {
    ClockEnable = enable;
}

void Chip4017::reset() {
    state = 0;
    updateOutputs();
}

void Chip4017::clock(bool level) {
    // ativa apenas na borda de subida
    if (!lastClock && level && ClockEnable) {
        // É como se o pino Q estivesse conectado ao pino do reset
        // então se o Q3 estiver no reset, o contator vai até Q2
        state = (state + 1) % Qreset; 
        updateOutputs();
    }
    lastClock = level;
}

void Chip4017::updateOutputs() {
    // ativa apenas o bit correspondente, da esquerda para a direita
    Out = 1u << (Qreset - 1 - state);
}

uint32_t Chip4017::getOut() const {
    return Out;
}