/*
[PT-BR]
    Teste do CI CD4040 (contador de ondulação de 12 estágios). A cada pressionamento da tecla 'ENTER', um pulso de clock é 
    aplicado ao chip, e o estado atual de suas saídas (Q0–Q11) é exibido no terminal.

    O programa permite observar o comportamento do divisor de frequência, onde cada saída representa uma divisão sucessiva 
    do clock de entrada.

[EN-US]
    Test for the CD4040 IC (12-stage ripple counter). Each time the ENTER key is pressed, a clock pulse is applied to the chip,
    and the current state of its outputs (Q0–Q11) is displayed in the terminal.

    This program allows observation of the frequency division behavior, where each output represents a successive division of 
    the input clock.
*/

// libraries
#include "chips.hpp"
#include <iostream>
#include <cstdlib>

// displays the current output state of the CD4040
void printChip(const Chip4040& chip, const std::string& name) {
    std::cout << name << ": ";
    for (int i = 11; i >= 0; i--) {
        std::cout << chip.getOutput(i);
    }
    std::cout << std::endl;
}

int main() {
    Chip4040 chip;

    // main loop
    while (true) {
        system("clear");
        printChip(chip, "Chip4040");

        std::cout << "\nPress 'ENTER' to generate the next clock pulse";
        std::cin.get();

        chip.clock();
    }
}