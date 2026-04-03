/*
[PT-BR]
    Teste individual do chip CD4017. Este programa exibe o estado atual das saídas do contador Johnson (CD4017) 
    e permite gerar manualmente o próximo pulso de clock pressionando 'ENTER'.

[EN-US]
    Individual test for the CD4017 chip. This program shows the current output state of the Johnson counter (CD4017)
    and allows manually generating the next clock pulse by pressing 'ENTER'.
*/

// libraries
#include "chips.hpp"
#include <iostream>
#include <cstdlib>


// Displays the current output state of the CD4017
void printChip(const Chip4017& chip, const std::string& name) {
    std::cout << name << ": ";
    unsigned n = chip.getLimitReset();

    for (int i = n-1; i >= 0; i--) {
        std::cout << chip.getOutput(i);
    }
    std::cout << std::endl;
}


int main() {
    const unsigned NUM_LAMPS = 2;
    Chip4017 chip(NUM_LAMPS, true);

    // main loop
    while (true) {
        system("clear");
        printChip(chip, "CD4017");

        std::cout << "\nPress 'ENTER' to generate the next clock pulse";
        std::cin.get();

        chip.shift();
    }

    // finished
    return 0;
}