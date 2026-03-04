#ifndef CHIPS_HPP
#define CHIPS_HPP
#include <cstdint>

class Chip4017 {
private:
    uint8_t state{0};           // posição atual do contador
    uint32_t Out{0};            // saídas Q0–Q9
    uint8_t Qreset{10};         // limite de contagem (quantos QPins usar no projeto)
    bool lastClock{false};      // para detectar borda de subida
    bool ClockEnable{true};     // habilita/desabilita a contagem

    void updateOutputs();       // atualiza o valor de Out

public:
    explicit Chip4017(uint8_t Qreset = 10);

    void setClockEnable(bool enable);
    void reset();
    void clock(bool level);

    uint32_t getOut() const;
};

#endif