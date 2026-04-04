/*
[PT-BR]
    O Digital Clockwork simula a lógica do circuito de um relógio digital inspirado no projeto de Wagner Rambo (WR Kits). 
    A classe DigitalClockwork orquestra a interação entre contadores BCD (CD4029), decodificadores de sete segmentos (CD4511), 
    portas AND (CD4081), um contador Johnson (CD4017), um contador de ondulação (CD4040) e um gerador de frequência.

    Cada chip foi modelado individualmente em uma classe separada, declarada em chips.hpp e implementada em chips.cpp. O gerador 
    de frequência foi modelado com o uso de threads para maior precisão temporal e, assim como os chips, foi encapsulado em sua 
    própria classe, declarada em freqGenerator.hpp e implementada em freqGenerator.cpp. Os arquivos feedback.hpp e feedback.cpp
    foram usados, respectivamente, para a declaração e implementação dos artifícios responsáveis pelo feedback visual do usuário 
    no terminal.

[EN-US]
    A Digital Clockwork simulates the logic of a digital clock circuit inspired by the project of Wagner Rambo (WR Kits).
    The class "DigitalClockwork" orchestrates the interaction between BCD counters (CD4029), seven-segment decoders (CD4511), 
    AND gates (CD4081), a Johnson counter (CD4017), a ripple counter (CD4040), and a frequency generator.

    Each chip was individually modeled in a separate class, declared in *chips.hpp* and implemented in *chips.cpp*. The frequency 
    generator was modeled using threads to achieve greater timing precision and, like the chips, was encapsulated in its own class, 
    declared in *freqGenerator.hpp* and implemented in *freqGenerator.cpp*. The files *feedback.hpp* and *feedback.cpp* were used, 
    respectively, for the declaration and implementation of the mechanisms responsible for providing visual feedback to the user in 
    the terminal.


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

// Libraries
#include "freqGenerator.hpp"
#include "feedback.hpp"
#include "chips.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <array>



// DigitalClockwork board class 
class DigitalClockwork{
    // Enum class placed here so it can be accessed by all encapsulated methods
    public:
        enum class ADJUSTMENT{                          
            FAST,                                       // Press 'F' to select Fast mode 
            SLOW,                                       // Press 'S' to select Slow mode 
            DEFAULT                                     // Default .-.
        };

    private:
        std::array<Chip4029*, 4> cd4029;
        std::array<Chip4511*, 4> cd4511;
        std::array<Chip4081*, 2> cd4081;
        Chip4017* cd4017;
        Chip4040* cd4040;

        // Edge detection flags for frequency conversion
        bool lastQ0 = false;       
        bool lastQ5 = false;           
        bool lastOut4081 = false; 
        bool meridienEdge = false;

        // contador para a gambiarra
        int defaultCooldown = 0;


        // Simulates the ripple carry between the minute counters
        void cascadeCount() {
            cd4029[0]->setCarryIn(true);
            cd4029[0]->clock();
        
            // cd4029[1] only advances when cd4029[0] overflows
            if(cd4029[0]->getCarryOut()){
                cd4029[1]->setCarryIn(true);
                cd4029[1]->clock();
            }
        }


        // Updates all four AND gates of cd4081[0].
        void updateAND() {
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

        // Applies resets and special clock pulses based on AND gate outputs.
        void applyResets() {
            if(cd4081[0]->getOutput(0)){
                cd4029[1]->setPresetEnable();
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

        // updating the seven-segment display state
        void updateDisplays(){
            for(int i = 0; i < 4; i++){
                std::array<bool, 4> intermediate;
                for(int j = 0; j < 4; j++){
                    intermediate[j] = cd4029[i]->getOutput(j);
                }
                cd4511[i]->setInputs(intermediate);
            }
        }


        // Orchestrates the full update cycle in the correct order
        void updateCounter() {
            cascadeCount();
            updateAND();
            applyResets();
            updateAND();

            // Ensures hour reset is applied before display update
            if(cd4081[0]->getOutput(2)){
                cd4029[2]->setPresetEnable();
                cd4029[3]->setPresetEnable();
                cd4029[2]->setCarryIn(true);
                cd4029[2]->clock();
            }

            updateDisplays();
        }


        // Updates all four AND gates of cd4081[1].
        void updateAND_1() {
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
        void frequencyConverter(ADJUSTMENT MODE) {
        
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


    public:
        DigitalClockwork(
            // Use arrays instead of pointers makes the constructor feels safe
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
        }

        // Advances the system clock and updates the circuit based on the selected mode
        void updateSystem(ADJUSTMENT MODE = ADJUSTMENT::DEFAULT){
            cd4040->clock();
            frequencyConverter(MODE);
        }


        // Returns the current seven-segment output state of all four display digits.
        std::array<std::array<bool, 7>, 4> getSegmentsOutput(){
            std::array<std::array<bool, 7>, 4> table = {{}};

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 7; j++){
                    table[i][j] = cd4511[i]->getSegmentsOut(j);
                }
            }
            return table;
        }

        bool getMeridien(size_t value){
            return cd4017->getOutput(value);
        }
};




int main(){
    // initial preset value for all BCD counters
    const std::array<bool, 4> presetZero = {0,0,0,0};

    
    // instantiate BCD counters, decoders and AND gate chips
    std::array<Chip4029, 4> cd4029 = {
        Chip4029(presetZero),
        Chip4029(presetZero),
        Chip4029(presetZero),
        Chip4029(presetZero)
    };

    std::array<Chip4511, 4> cd4511{};
    std::array<Chip4081, 2> cd4081{};  
                                                        
    FreqGenerator clk;                                  // frequency: 60 Hz       
    Chip4017 cd4017(2, true);                           // AM/PM indicator
    Chip4040 cd4040;                                    // 12-stage ripple counter — frequency divider


    // feedback components
    Display display;
    Led AM;
    Led PM;

    
    // create pointer arrays to pass chips into DigitalClockwork
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


    // instantiate the main clockwork controller
       DigitalClockwork functions(
        ptr4029,
        ptr4511,
        ptr4081,
        &cd4017,
        &cd4040  
    );


    // configure all counters and decoders             
    for(int i = 0; i < 4; i++){
        cd4511[i].setLampTest(true);                    // enable normal display output
        cd4511[i].setBlanking(true);                    // enable segment output
        cd4029[i].setBinaryDecade(false);               // set to decimal (BCD) mode
        cd4029[i].setUpDown(true);                      // set to count up
    }


    // start the 60Hz frequency generator
    clk.start();
    bool lastState = clk.getState();


    // updates the clock on each rising edge of the frequency generator
    while(true) {
        clk.waitEdge(lastState);
        bool curState = clk.getState();

        if(!lastState && curState){
            system("clear");
            functions.updateSystem(DigitalClockwork::ADJUSTMENT::DEFAULT);  // <- Select clock speed mode (FAST, SLOW, DEFAULT) here

            std::array<std::array<bool, 7>, 4> segments = functions.getSegmentsOutput();

            AM.setState(functions.getMeridien(0));
            PM.setState(functions.getMeridien(1));

            std::cout << display.render(segments) << " | "; 
            std::cout << "AM:" << AM.getState() << "  PM:" << PM.getState() << " |\n";        
        }
        lastState = curState;
    }


    // stop the frequency generator and finish this program
    clk.stop();
    return 0;
}