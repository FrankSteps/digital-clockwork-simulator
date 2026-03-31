/*
[PT-BR]


[EN-US]
    
*/

// LIBRARIES
#include "freqGenerator.hpp"
#include "chips.hpp"
#include <iostream>
#include <string.h>
#include <array>


// Desktop
class DigitalClockwork{
    private:
        std::array<Chip4029*, 4> cd4029;
        std::array<Chip4511*, 4> cd4511;
        std::array<Chip4081*, 2> cd4081;
        Chip4017* cd4017;
        Chip4040* cd4040;
        FreqGenerator* clock;

        void updateCounter() {
            bool carryIn = true;
        
            for(int i = 0; i < 4; i++){
                cd4029[i]->setCarryIn(carryIn);
                cd4029[i]->clock();
                carryIn = cd4029[i]->getCarryOut();

                std::array<bool, 4> intermediate;
                for(int j = 0; j < 4; j++){
                    intermediate[j] = cd4029[i]->getOutput(j);
                }
                cd4511[i]->setInputs(intermediate);
            }
        
            // set inputs values
            std::array<bool, 4> inputA{}, inputB{};

            inputA[0] = cd4029[1]->getOutput(1);
            inputB[0] = cd4029[1]->getOutput(2);

            inputA[1] = cd4029[2]->getOutput(0);
            inputB[1] = cd4029[2]->getOutput(1);

            inputA[2] = cd4029[3]->getOutput(0);
            inputB[2] = cd4081[0]->getOutput(1);


            /*
                To AM-PM logic
                when exist a counter overflow in both cd4029 counters a clock is applied in cd4017 (Johnson counter)
            */
            inputA[3] = cd4029[2]->getOutput(1);
            inputB[3] = cd4029[3]->getOutput(0);
        

            // Update cd4081 
            for(int k = 0; k < 4; k++){
                cd4081[0]->setInputA(k, inputA[k]);
                cd4081[0]->setInputB(k, inputB[k]);
            }
    

            // Resets
            if(cd4081[0]->getOutput(0)){
                cd4029[0]->setPresetEnable(); 
                cd4029[1]->setPresetEnable();
            }
        
            if(cd4081[0]->getOutput(1)){
                cd4029[2]->setPresetEnable();
            }
        
            if(cd4081[0]->getOutput(2)){
                cd4029[2]->setPresetEnable();
                cd4029[3]->setPresetEnable();
            }

            if(cd4081[0]->getOutput(3)){
                cd4017->shift();
            }
        }



    public:
        DigitalClockwork(
            // Use arrays instead of pointers makes the constructor feel safe 
            std::array<Chip4029*, 4> chips4029,
            std::array<Chip4511*, 4> chips4511,
            std::array<Chip4081*, 2> chips4081,
            Chip4017* chip4017,
            Chip4040* chip4040,
            FreqGenerator* clk
        ){
            for(int i = 0; i < 4; i++){
                cd4029[i] = chips4029[i];
                cd4511[i] = chips4511[i];
            }

            for(int i = 0; i < 2; i++){
                cd4081[i] = chips4081[i];
            }

            clock = clk;
            cd4017 = chip4017;
            cd4040 = chip4040;
        }
        

        void updateSystem(){
            updateCounter();
        }
};



int main(){
    // CONSTANTS
    const std::array<bool, 4> presetZero = {0,0,0,0};

    
    // CREATE OBJECTS
    std::array<Chip4029, 4> cd4029 = {
        Chip4029(presetZero),
        Chip4029(presetZero),
        Chip4029(presetZero),
        Chip4029(presetZero)
    };

    std::array<Chip4511, 4> cd4511{};  
    std::array<Chip4081, 2> cd4081{};  

    FreqGenerator clk;            // frequency: 60 Hz       
    Chip4017 cd4017(2, true);     // AM/PM indicator
    Chip4040 cd4040; 

    
    // CREATE POINTERS ARRAYS
    std::array<Chip4029*, 4> ptr4029;
    std::array<Chip4511*, 4> ptr4511;
    std::array<Chip4081*, 2> ptr4081;

    for(int i = 0; i < 4; i++){
        ptr4029[i] = &cd4029[i];
        ptr4511[i] = &cd4511[i];
    }

    for(int i = 0; i < 2; i++){
        ptr4081[i] = &cd4081[i];
    }


    // ADD THE CIs IN THE DIGITAL CLOCKWORK FUNCTIONS CLASS
       DigitalClockwork functions(
        ptr4029,
        ptr4511,
        ptr4081,
        &cd4017,
        &cd4040,
        &clk   
    );
    

    // SETTINGS 
    for(int i = 0; i < 4; i++){
        cd4511[i].setLampTest(true);
        cd4511[i].setBlanking(true);
        cd4029[i].setBinaryDecade(false);
        cd4029[i].setUpDown(true);
    }

    // END
    return 0;
}