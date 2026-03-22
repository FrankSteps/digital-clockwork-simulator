#include "chips.hpp"
#include <iostream>
#include <cstdlib>  // system("clear")

void printChip(const Chip4040& chip, const std::string& name) {
    std::cout << name << ": ";
    for (int i = 11; i >= 0; i--) {
        std::cout << chip.getOutput(i);
    }
    std::cout << std::endl;
}

int main() {
    Chip4040 chip;

    while (true) {
        system("clear");
        printChip(chip, "Chip4040");

        std::cout << "\n Pressione ENTER para gerar próximo pulso de clock...";
        std::cin.get();

        chip.clock();
    }

    return 0;
}