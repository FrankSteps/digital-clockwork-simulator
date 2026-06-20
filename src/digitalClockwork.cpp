/*
[DO-CS] .-.
    .-----------------------------------------------------------------------------------------.
    | cd4029[0]:  unit of minutes  |  cd4511[0]:   seven-segment decoders to unit of minutes  | 
    | cd4029[1]:  tens of minutes  |  cd4511[1]:   seven-segment decoders to tens of minutes  |
    | cd4029[2]:  unit of hours    |  cd4511[2]:   seven-segment decoders to unit of hours    |
    | cd4029[3]:  tens of hours    |  cd4511[3]:   seven-segment decoders to tens of hours    |
    |-----------------------------------------------------------------------------------------|
    | cd4081[0]:  to counter logic            | cd4017:     AM-PM indicator                   |
    | cd4081[1]:  to frequence divide logic   | cd4040:     frequency divider                 |
    |-----------------------------------------------------------------------------------------|
    | frequency generator: 60Hz                                                               |
    '-----------------------------------------------------------------------------------------'
*/

#include "digitalClockwork.hpp"
#include "chips.hpp"


//  Simulates the ripple carry between the minute counters
void DigitalClockwork::cascadeCount(){
    cd4029[0]->setCarryIn(true);
    cd4029[0]->clock();
    
    // cd4029[1] only advances when cd4029[0] overflows
    if(cd4029[0]->getCarryOut()){
        cd4029[1]->setCarryIn(true);
        cd4029[1]->clock();
    } 
}


//  Updates all four AND gates of cd4081[0].
void DigitalClockwork::updateAND(){
    std::array<bool, 4> inputA{}, inputB{};

    inputA[0] = cd4029[1]->getOutput(1);        // tens of minutes bit 1
    inputB[0] = cd4029[1]->getOutput(2);        // tens of minutes bit 2    -> detects 6  

    inputA[1] = cd4029[2]->getOutput(0);        // tens of minutes bit 2    -> detects 6
    inputB[1] = cd4029[2]->getOutput(1);        // unit of hours bit 1      -> detects 3

    inputA[2] = cd4081[0]->getOutput(1);        // gate 1 output (cascade)
    inputB[2] = cd4029[3]->getOutput(0);        // tens of hours bit 0      -> detects 13

    inputA[3] = cd4029[2]->getOutput(1);        // unit of hours bit 1  
    inputB[3] = cd4029[3]->getOutput(0);        // tens of hours bit 0      -> detects 12

    for(int k = 0; k < 4; k++){
        cd4081[0]->setInputA(k, inputA[k]);
        cd4081[0]->setInputB(k, inputB[k]);
    }   
}


//  Applies resets and special clock pulses based on AND gate outputs
void DigitalClockwork::applyResets(){
    if(cd4081[0]->getOutput(0)){
        cd4029[1]->setPresetEnable();
        cd4029[1]->clock();
        cd4029[2]->setCarryIn(true);
        cd4029[2]->clock();

        // propagate carry to tens of hours if unit of hours overflowed
        if(cd4029[2]->getCarryOut()){
            cd4029[3]->setCarryIn(true);
            cd4029[3]->clock();
        }

        // recalculate AND gates after the clock pulse
        updateAND(); 

        // if hours reached 13, reset both hour counters and clock unit to 1
        if(cd4081[0]->getOutput(2)){
            cd4029[2]->setPresetEnable();       // reset unit of hours to 0
            cd4029[3]->setPresetEnable();       // reset tens of hours to 0
            cd4029[2]->setCarryIn(true);    
            cd4029[2]->clock();                 // clock unit of hours from 0 to 1
        }
    }

    /*
        To AM-PM logic
        when exist a counter overflow in both cd4029 counters a clock is applied in cd4017 (Johnson counter)
    */
    bool currentOutput3 = cd4081[0]->getOutput(3);
    if(currentOutput3 && !meridienEdge){
        cd4017->shift();
    }
    meridienEdge = currentOutput3;
}


//  updating the seven-segment display state
void DigitalClockwork::updateDisplays(){
    for(int i = 0; i < 4; i++){
        std::array<bool, 4> intermediate;
        for(int j = 0; j < 4; j++){
            intermediate[j] = cd4029[i]->getOutput(j);
        }
        cd4511[i]->setInputs(intermediate);
    }
}


//  Orchestrates the full update cycle in the correct order
void DigitalClockwork::updateCounter(){
    cascadeCount();
    updateAND();
    applyResets();
    updateAND();

    // Ensures hour reset is applied before display update
    if(cd4081[0]->getOutput(2)){
        cd4029[2]->setPresetEnable();
        cd4029[3]->setPresetEnable();
        cd4029[3]->clock();
        cd4029[2]->setCarryIn(true);
        cd4029[2]->clock();
    }

    updateDisplays();
}


//  Updates all four AND gates of cd4081[1]
void DigitalClockwork::updateAND_1(){
    std::array<bool, 3> inputA{}, inputB{}; 

    inputA[0] = cd4040->getOutput(4); 
    inputB[0] = cd4040->getOutput(11);

    inputA[1] = cd4040->getOutput(9); 
    inputB[1] = cd4040->getOutput(10);

    inputA[2] = cd4081[1]->getOutput(0);
    inputB[2] = cd4081[1]->getOutput(1);

    for(int l = 0; l < 3; l++){
        cd4081[1]->setInputA(l, inputA[l]);
        cd4081[1]->setInputB(l, inputB[l]);
    }    
}


/*
    Acquires the clock signal from cd4040 and divides its frequency
    to control the counting speed of the clock
*/
void DigitalClockwork::frequencyConverter(ADJUSTMENT MODE){
    // read divided clock signals
    bool Q0 = cd4040->getOutput(0);
    bool Q5 = cd4040->getOutput(5);

    updateAND_1();
    bool Out4081 = cd4081[1]->getOutput(2);
        
    switch(MODE){
        case ADJUSTMENT::FAST:
            if(Q0 && !lastQ0){
                updateCounter();
            }
            break;
        
        case ADJUSTMENT::SLOW:
            if(Q5 && !lastQ5){
                updateCounter();
            }
            break;
                
        /*
            Isso aqui foi uma gambiarra temporária. 
            Planejo desenvolver algum artifício que elimine o ruido da saída da AND gate no cd4081[1] (funcionando como um capacitor, por exemplo).
            
            Por enquanto, adotei a medida "software-like"
        */
        case ADJUSTMENT::DEFAULT:
            if(Out4081 && !lastOut4081 && defaultCooldown == 0){
                updateCounter();
                defaultCooldown = 500;
            }

            if(defaultCooldown > 0){
                defaultCooldown--;
            }

            lastOut4081 = Out4081;
            break;
    }
    lastQ0 = Q0;
    lastQ5 = Q5;
    lastOut4081 = Out4081;
}



DigitalClockwork::DigitalClockwork(
    //  Use arrays instead of pointers makes the constructor feels safe
    std::array<Chip4029*, 4> chips4029,
    std::array<Chip4511*, 4> chips4511,
    std::array<Chip4081*, 2> chips4081,
    Chip4017* chip4017,
    Chip4040* chip4040
){
    for(int i = 0; i < 4; i++){
        cd4029[i] = chips4029[i];
        cd4511[i] = chips4511[i];
    }

    for(int i = 0; i < 2; i++){
        cd4081[i] = chips4081[i];
    }

    cd4017 = chip4017;
    cd4040 = chip4040;

    updateDisplays();
}


//  Advances the system clock and updates the circuit based on the selected mode
void DigitalClockwork::updateSystem(ADJUSTMENT MODE){
    cd4040->clock();
    frequencyConverter(MODE);
}


//  Returns the current seven-segment output state of all four display digits
std::array<std::array<bool, 7>, 4> DigitalClockwork::getSegmentsOutput(){
    std::array<std::array<bool, 7>, 4> table = {{}};

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 7; j++){
            table[i][j] = cd4511[i]->getSegmentsOut(j);
        }
    }
    return table;
}


// AM or PM?
bool DigitalClockwork::getMeridien(size_t value){
    return cd4017->getOutput(value);
}