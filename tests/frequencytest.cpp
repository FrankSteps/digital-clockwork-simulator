/*
[PT-BR]
    Este programa foi criado para testar e demonstrar o uso do FreqGenerator. O FreqGenerator gera o sinal
    de clock que alimenta os circuitos integrados CD4040 (divisor de frequência) e CD4029 (contador).

    No loop principal, é feita a detecção de borda de subida do sinal de clock para sincronizar a atualização 
    dos circuitos. A cada borda de subida, o CD4040 é acionado, e sua saída (Q5) é usada como um clock dividido 
    para o CD4029.

    Dessa forma, o CD4040 reduz a frequência do sinal original, e o CD4029 conta com base nessa frequência mais 
    lenta. A função auxiliar printChip exibe o estado atual das saídas do CD4029.

[EN-US]
    This program was created to test and demonstrate the usage of the FreqGenerator. The FreqGenerator produces 
    a clock signal that drives the CD4040 (frequency divider) and the CD4029 (counter).

    In the main loop, rising edge detection is used to synchronize circuit updates. On each rising edge of the 
    clock signal, the CD4040 is triggered, and its Q5 output is used as a divided clock signal for the CD4029.

    This way, the CD4040 reduces the original clock frequency, and the CD4029 counts based on this slower signal.
    The helper function printChip displays the current output state of the CD4029.
*/

// libraries 
#include "chips.hpp"
#include "freqGenerator.hpp"
#include <iostream>

// displays the current output state of the CD4029
void printChip(const Chip4029& chip, const std::string& name) {
    std::cout << name << ": ";
    for (int i = 3; i >= 0; i--) {
        std::cout << chip.getOutput(i);
    }
}

int main() {
    FreqGenerator clk;
    Chip4040 cd4040;
    Chip4029 cd4029({0,0,0,0}); 

    cd4029.setBinaryDecade(true); 
    cd4029.setUpDown(false);      

    // start the 60Hz frequency generator
    clk.start();

    // flags
    bool lastState = clk.getState(); 
    bool last4040State = cd4040.getOutput(5); 

    // updates the clock on each rising edge of the frequency generator
    while(true) {
        clk.waitEdge(lastState);
        bool curState = clk.getState();

        if(!lastState && curState){ 
            cd4040.clock();

            bool q5 = cd4040.getOutput(5);
            if(!last4040State && q5){ 
                cd4029.clock(); 
                printChip(cd4029, "Chip_4029");
            }
            last4040State = q5;
        }
        lastState = curState;
    }

    // stop the frequency generator and finish this program test
    clk.stop();
    return 0;
}