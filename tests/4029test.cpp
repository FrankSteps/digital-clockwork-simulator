/*
[PT-BR]
    Teste de dois CIs CD4029 em cascata. Cada vez que a tecla ENTER é pressionada, um pulso de clock é aplicado ao 
    primeiro chip. Se o primeiro chip gerar um Carry Out, o segundo chip recebe um pulso de clock, simulando a 
    contagem em cascata. O estado atual de cada chip e seu Carry Out são exibidos no terminal.

[EN-US]
    Test for two CD4029 ICs in cascade. Each time the ENTER key is pressed, a clock pulse is applied to the first 
    chip. If the first chip generates a Carry Out, the second chip receives a clock pulse, simulating cascade counting.
    The current state of each chip and its Carry Out are displayed in the terminal.
*/

// libraries
#include "chips.hpp"
#include <iostream>
#include <cstdlib>


// displays the current output state of the CD4029
void printChip(const Chip4029& chip, const std::string& name) {
    std::cout << name << ": ";
    for (int i = 3; i >= 0; i--) {
        std::cout << chip.getOutput(i);
    }
    std::cout << "  Carry Out: " << chip.getCarryOut() << std::endl;
}

int main() {
    // Two 4029 chips in cascade 
    Chip4029 chip1({0,0,0,0});     
    Chip4029 chip2({0,0,0,0});     
    
    chip1.setBinaryDecade(true);  // decade mode
    chip2.setBinaryDecade(true);  // decade mode
    chip1.setUpDown(false);       // up
    chip2.setUpDown(false);       // up

    // main loop
    while (true) {
        // clear the terminal
        system("clear");

        // carryOut 
        if (chip1.getCarryOut()) {
            chip2.clock();
        }

        // print state
        printChip(chip1, "Chip1");
        printChip(chip2, "Chip2");

        std::cout << "\nPress 'ENTER' to generate the next clock pulse";
        std::cin.get();  // wait for enter

        // Trigger clock on the first chip
        chip1.clock();
    }

    // finished
    return 0;
}