#include "chips.hpp"
#include <iostream>
#include <cstdlib>  // system("clear")

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

        std::cout << "\nPressione ENTER para gerar próximo pulso de clock...";
        std::cin.get();  // wait for enter

        // Trigger clock on the first chip
        chip1.clock();
    }

    return 0;
}