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

    A leitura de teclado é feita por trás da interface declarada em keyboard.hpp, implementada uma vez por 
    sistema operacional (keyboard_linux.cpp, keyboard_windows.cpp, keyboard_macos.cpp). Apenas um desses arquivos 
    é compilado por vez, escolhido pelo Makefile de acordo com o SO detectado.

[EN-US]
    A Digital Clockwork simulates the logic of a digital clock circuit inspired by the project of Wagner Rambo (WR Kits).
    The class "DigitalClockwork" orchestrates the interaction between BCD counters (CD4029), seven-segment decoders (CD4511), 
    AND gates (CD4081), a Johnson counter (CD4017), a ripple counter (CD4040), and a frequency generator.

    Each chip was individually modeled in a separate class, declared in *chips.hpp* and implemented in *chips.cpp*. The frequency 
    generator was modeled using threads to achieve greater timing precision and, like the chips, was encapsulated in its own class, 
    declared in *freqGenerator.hpp* and implemented in *freqGenerator.cpp*. The files *feedback.hpp* and *feedback.cpp* were used, 
    respectively, for the declaration and implementation of the mechanisms responsible for providing visual feedback to the user in 
    the terminal.

    Keyboard reading is done behind the interface declared in keyboard.hpp, implemented once per operating 
    system (keyboard_linux.cpp, keyboard_windows.cpp, keyboard_macos.cpp). Only one of these files is compiled 
    at a time, chosen by the Makefile based on the detected OS.
*/

//libraries
#include "digitalClockwork.hpp"                         // The Wagner Rambo's original project simulation
#include "digitalAlarm.hpp"                             // the extension: The alarm system for The Digital Clockwork

#include "chips.hpp"                                    // CD4029, CD4511, CD4081, CD4017, CD4040...
#include "feedback.hpp"                                 // Display, Led...
#include "audio_output.hpp"                             // Buzzer, Speaker...
#include "freqGenerator.hpp"                            // Frequency generator
#include "keyboard.hpp"                                 // platform keyboard interface

#include <iostream>                                     // std::cout, std::cerr
#include <cstdlib>                                      // system()
#include <string>                                       // std::string
#include <array>                                        // std::array


int main(int argc, char* argv[]){
    /*
    
        COMUNICATION
        KEYBOARD EVENT: PLATFORM-SPECIFIC (see keyboard.hpp) + PATH TO .WEEK
    
    */

    initKeyboard();


    // path to file.week?
    if(argc < 2){
        std::cerr << "usage: ./digital-clockwork <path/to/file.week>\n";
        return EXIT_FAILURE;
    }


    /*
    
        INSTANTIATE:
        FEEDBACK COMPONENTS
    
    */

    Display display;
    Led AM; 
    Led PM;

    std::array<Led, 7> days; 
    const char* dayNames[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

    Led ALARMLED;

    Buzzer buzzer;
    buzzer.start();

    /*

        INSTANTIATE: 
        A DIGITAL CLOCKWORK

    */

    DigitalClockwork clockwork;


    /*
    
        INSTANTIATE: 
        THE AMAZING DIGITAL ALARM
    
    */

    DigitalAlarm alarm(argv[1]);


    /*

        RUN:
        PROGRAMM
    
    */
    
    // frequency generator: 60 Hz
    FreqGenerator clk;      


    // start the 60Hz frequency generator
    clk.start();
    bool lastState = clk.getState();

    DigitalClockwork::ADJUSTMENT mode = DigitalClockwork::ADJUSTMENT::DEFAULT;

    // terminal config
    terminalSetup();


    // key edge tracking, replaces the (code, value) pair from the raw event sources
    KeyState lastKeys = {false, false, false, false, false, false};


    // Meridiem edge flag
    bool lastMeridiem = false;


    // updates the clock on each rising edge of the frequency generator
    while(true) {
        // read keyboard input: current state of each tracked key, platform-specific
        KeyState curKeys = pollKeys();

        // key pressed (rising edge)
        if(curKeys.F && !lastKeys.F){
            mode = DigitalClockwork::ADJUSTMENT::FAST;
        }

        if(curKeys.S && !lastKeys.S){
            mode = DigitalClockwork::ADJUSTMENT::SLOW;
        }

        if(curKeys.P && !lastKeys.P){
            alarm.programAlarm();
        }

        if(curKeys.A && !lastKeys.A){
            alarm.advanceDay();
        }

        if(curKeys.R && !lastKeys.R){
            alarm.reset();
        }

        if(curKeys.D && !lastKeys.D){
            alarm.disarm();
        }

        // key released (falling edge)
        if((!curKeys.F && lastKeys.F) || (!curKeys.S && lastKeys.S)){
            mode = DigitalClockwork::ADJUSTMENT::DEFAULT;
        }

        lastKeys = curKeys;

        // wait for rising edge and update the circuit
        clk.waitEdge(lastState);
        bool curState = clk.getState();

        if(curState && !lastState){
            std::cout << "\033[H";
            clockwork.updateSystem(mode);

            std::array<std::array<bool, 7>, 4> segments = clockwork.getSegmentsOutput();
            alarm.setClockSignal(clockwork.get4040FrequencyQ6());

            AM.setState(clockwork.getMeridien(0));
            PM.setState(clockwork.getMeridien(1));

            bool curMeridiem = clockwork.getMeridien(0);

            if(curMeridiem && !lastMeridiem){
                alarm.advanceDay();
                alarm.rearmStandby();
            }
            lastMeridiem = curMeridiem;

            alarm.setMeridiem(clockwork.getMeridien(0));
            alarm.setDataMemory(clockwork.getCountersOutput());

            std::cout << display.render(segments) << " | "; 
            std::cout << "AM:" << AM.getState() << "  PM:" << PM.getState();

            ALARMLED.setState(alarm.getOutputSystem());
            std::cout << " | ALARM:" << ALARMLED.getState() << "\n"; 
            
            for(int i = 0; i < 7; i++){
                days[i].setState(alarm.getCurrentDay(i));
                std::cout << dayNames[i] << ":" << days[i].getState() << "  ";
            }

            buzzer.setState(alarm.getOutputSystem());
        }
        
        lastState = curState;
    }



    /*

        FINISH:
        PROGRAMM
    
    */

    // stop the frequency generator, close the keyboard capture and restore the terminal
    clk.stop();
    closeKeyboard();
    terminalTeardown();

    return EXIT_SUCCESS;
}