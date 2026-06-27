/*
[PT-BR]
    Teste individual do chip CD4013. Este programa testa os dois flip-flops D do CD4013,
    permitindo observar o comportamento das saídas Q e Q̄ ao aplicar Set, Reset e Clock
    manualmente pressionando 'ENTER' para avançar pelos casos de teste.

[EN-US]
    Individual test for the CD4013 chip. This program tests both D flip-flops of the CD4013,
    allowing observation of the Q and Q̄ output behavior when applying Set, Reset and Clock
    manually by pressing 'ENTER' to advance through the test cases.
*/

// libraries
#include "chips.hpp"
#include <iostream>
#include <cstdlib>


// displays the current state of the CD4013
void printChip(const Chip4013& chip, const std::string& name) {
    std::cout << name << ":";
    std::cout << "  FF0: Q=" << chip.getOutput(0) << " Q̄=" << chip.getNegOutput(0);
    std::cout << "  FF1: Q=" << chip.getOutput(1) << " Q̄=" << chip.getNegOutput(1) << std::endl;
}


int main() {
    Chip4013 chip;

    // test cases: {flipflop, operation, data}
    // operations: 0=set, 1=reset, 2=clock
    struct TestCase {
        size_t flipflop;
        int operation;
        bool data;
        std::string description;
    };

    std::array<TestCase, 6> cases {{
        {0, 0, false, "FF0: set"},
        {0, 1, false, "FF0: reset"},
        {0, 2, true,  "FF0: clock with data = 1"},
        {1, 0, false, "FF1: set"},
        {1, 1, false, "FF1: reset"},
        {1, 2, true,  "FF1: clock with data = 1"}
    }};

    int step = 0;

    // main loop
    while (true) {
        system("clear");

        TestCase& tc = cases[step];

        if (tc.operation == 0) {
            chip.set(tc.flipflop);
        } else if (tc.operation == 1) {
            chip.reset(tc.flipflop);
        } else {
            chip.setData(tc.flipflop, tc.data);
            chip.clock(tc.flipflop);
        }

        std::cout << "Step: " << tc.description << std::endl;
        printChip(chip, "CD4013");

        std::cout << "\nPress 'ENTER' to next test case";
        std::cin.get();

        step = (step + 1) % 6;
    }

    // finished
    return 0;
}