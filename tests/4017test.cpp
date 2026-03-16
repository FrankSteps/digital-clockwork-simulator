#include "chips.hpp"
#include <iostream>
#include <cstdlib>  // system("clear")

void printChip(const Chip4017& chip, const std::string& name) {
    std::cout << name << ": ";
    unsigned n = chip.getLimitReset();
    for (int i = n-1; i >= 0; --i) {
        std::cout << chip.getOutput(i);
    }
    std::cout << std::endl;
}

int main() {
    const unsigned NUM_LAMPS = 10;
    Chip4017 chip(NUM_LAMPS, true);  // ClockEnable = true

    while (true) {
        system("clear");
        printChip(chip, "CD4017");

        std::cout << "\nPressione ENTER para gerar próximo pulso de clock...";
        std::cin.get();

        chip.shift();
    }

    return 0;
}