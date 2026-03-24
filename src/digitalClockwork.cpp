/*
[PT-BR]


[EN-US]

*/

// LIBRARIES
#include "freqGenerator.hpp"
#include "chips.hpp"
#include <iostream>
#include <string.h>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <array>


// Desktop -> ambiente de trabalho
class DigitalClockworkFunctions{
    private:
        Chip4029* cd4029[4];
        Chip4511* cd4511[4];
        Chip4081* cd4081[2];

    public:
        DigitalClockworkFunctions(Chip4029* chips4029[4], Chip4511* chips4511[4], Chip4081* chips4081[2]){
            for(int i = 0; i < 4; i++){
                cd4029[i] = chips4029[i];
                cd4511[i] = chips4511[i];

                if(i % 2 == 0){
                    cd4081[i] = chips4081[i];
                }
            }
        }

        void update4511(){
            for(int i = 0; i < 4; i++){
                std::array<bool, 4> intermediate;

                for(int j = 0; j < 4; j++){
                    intermediate[j] = cd4029[i]->getOutput(j);
                }

                cd4511[i]->setInputs(intermediate);
            }
        }
};




int main(){
    // VECTORS
    std::vector<Chip4029> cd4029;               
    std::vector<Chip4511> cd4511;               
    std::vector<Chip4081> cd4081;    
    

    // POINTERS
    Chip4029* ptr4029[4];
    Chip4511* ptr4511[4];
    Chip4081* ptr4081[2];

    for (int i = 0; i < 4; i++) {
        ptr4029[i] = &cd4029[i];  
        ptr4511[i] = &cd4511[i];

        if(i % 2 == 0){
            ptr4081[i] = &cd4081[i];
        }
    }


    // CONSTANTS
    const std::array<bool, 4> presetZero = {0,0,0,0};


    // CREATE OBJECTS
    FreqGenerator clk;                     // frequency: 60 Hz       
    Chip4017 cd4017(2, true);              // AM/PM indicator
    Chip4040 cd4040;                       
    
    for(int i = 0; i < 4; i++) {
        cd4029.emplace_back(presetZero);
        cd4511.emplace_back(); 
        if(i % 2 == 0){
            cd4081.emplace_back();
        }
    }  

    // SETTINGS
    for(int i = 0; i < 4; i++){
        cd4511[i].setLampTest(true);
        cd4511[i].setBlanking(true);
        cd4029[i].setBinaryDecade(true);
        cd4029[i].setUpDown(false);
    }


    return 0;
}