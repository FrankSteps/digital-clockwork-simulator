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


int main(){
    // VECTORS
    std::vector<Chip4029> cd4029;               
    std::vector<Chip4511> cd4511;               
    std::vector<Chip4081> cd4081;               


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

    // CONFIGURATIONS
    for(int i = 0; i < 4; i++){
        cd4029[i].setBinaryDecade(true);
        cd4029[i].setUpDown(false);
    }

    return 0;
}