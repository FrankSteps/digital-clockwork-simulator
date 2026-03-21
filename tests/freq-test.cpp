#include "chips.hpp"
#include "freqGenerator.hpp"
#include <iostream>

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

    clk.start();

    bool lastState = clk.getState(); 
    bool last4040State = cd4040.getOutput(5); 

    while(true) {
        clk.waitEdge(lastState);
        bool curState = clk.getState();

        if(!lastState && curState){ 
            cd4040.clock();

            bool q5 = cd4040.getOutput(2);
            if(!last4040State && q5){ 
                cd4029.clock(); 
                printChip(cd4029, "Chip_4029");
            }
            last4040State = q5;
        }

        lastState = curState;
    }

    clk.stop();
}