/*
[PT-BR]
    Teste individual do chip CD4063. Este programa compara dois números de 4 bits (A e B) e exibe
    os resultados das saídas A>B, A=B e A<B. Os valores de A e B podem ser alterados manualmente
    pressionando 'ENTER' para avançar pelos casos de teste.
[EN-US]
    Individual test for the CD4063 chip. This program compares two 4-bit numbers (A and B) and displays
    the results of the outputs A>B, A=B and A<B. The values of A and B can be changed manually
    by pressing 'ENTER' to advance through the test cases.
*/

// libraries
#include "chips.hpp"
#include <iostream>
#include <cstdlib>

// displays the current state of the CD4063
void printChip(const Chip4063& chip, const std::string& name) {
    std::cout << name << ":  A>B: " << chip.getOutputGreater()
                      << "  A=B: " << chip.getOutputEqual()
                      << "  A<B: " << chip.getOutputSmaller() << std::endl;
}

int main() {
    Chip4063 chip;
    chip.setInputEqual(true);
    chip.setInputGreater(false);
    chip.setInputSmaller(false);

    // test cases: {A3,A2,A1,A0} vs {B3,B2,B1,B0}
    std::array<std::array<bool, 4>, 4> A {{
        {0,0,0,0},   // 0
        {0,1,0,1},   // 5
        {1,0,0,1},   // 9
        {1,1,1,1}    // 15
    }};
    std::array<std::array<bool, 4>, 4> B {{
        {0,0,0,0},   // 0
        {1,0,0,1},   // 9
        {0,1,0,1},   // 5
        {1,1,1,1}    // 15
    }};

    int step = 0;

    // main loop
    while (true) {
        system("clear");

        for (int i = 0; i < 4; i++) {
            chip.setInputA(i, A[step][i]);
            chip.setInputB(i, B[step][i]);
        }

        std::cout << "A: ";
        for (int i = 3; i >= 0; i--) std::cout << A[step][i];
        std::cout << "  B: ";
        for (int i = 3; i >= 0; i--) std::cout << B[step][i];
        std::cout << std::endl;

        printChip(chip, "CD4063");

        std::cout << "\nPress 'ENTER' to next test case";
        std::cin.get();

        step = (step + 1) % 4;
    }
    // finished
    return 0;
}