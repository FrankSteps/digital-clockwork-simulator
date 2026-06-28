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

[LINUX]
    This main.cpp is only for Linux users
*/

//libraries
#include "digitalClockwork.hpp"                         // The Wagner Rambo's original project simulation
#include "digitalAlarm.hpp"                             // the extension: The alarm system for The Digital Clockwork

#include "chips.hpp"                                    // CD4029, CD4511, CD4081, CD4017, CD4040...
#include "feedback.hpp"                                 // Display, Led...
#include "freqGenerator.hpp"                            // Frequency generator

#include <iostream>                                     // std::cout, std::cerr
#include <cstdlib>                                      // system()
#include <string>                                       // std::string
#include <array>                                        // std::array

#include <libevdev/libevdev.h>                          // keyboard input events
#include <fcntl.h>                                      // open(), O_RDONLY, O_NONBLOCK
#include <unistd.h>                                     // close()


int main(int argc, char* argv[]){
    /*
    
        COMUNICATION
        KEYBOARD EVENT: LINUX + PATH TO .WEEK
    
    */

    // open keyboard device
    int fd = open("/dev/input/event4", O_RDONLY | O_NONBLOCK);

    if(fd < 0){
        std::cerr << "The file couldn't open\n";
        return EXIT_FAILURE;
    }

    // initialize libevdev device from file descriptor
    struct libevdev* dev = nullptr;
    libevdev_new_from_fd(fd, &dev);


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
    system("clear");
    system("stty -echo");


    // Meridiem edge flag
    bool lastMeridiem = false;


    // updates the clock on each rising edge of the frequency generator
    while(true) {
        // read keyboard input
        struct input_event ev;

        if(libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev) == 0){
            if(ev.type == EV_KEY){
                // key pressed
                if(ev.value == 1){
                    // CLOCKWORK CONFIG KEYS
                    if(ev.code == KEY_F){
                        mode = DigitalClockwork::ADJUSTMENT::FAST;
                    }

                    if(ev.code == KEY_S){
                        mode = DigitalClockwork::ADJUSTMENT::SLOW;
                    }

                    // ALARM CONFIG KEYS
                    if(ev.code == KEY_P){ 
                        alarm.programAlarm(); 
                    }

                    if(ev.code == KEY_A){ 
                        alarm.advanceDay();   
                    }

                    if(ev.code == KEY_R){ 
                        alarm.reset();        
                    }

                    if(ev.code == KEY_D){ 
                        alarm.disarm();        
                    }
                }

                // key released
                if(ev.value == 0){
                    if(ev.code == KEY_F || ev.code == KEY_S){
                        mode = DigitalClockwork::ADJUSTMENT::DEFAULT;
                    }
                }
            }
        }
        
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

    // stop the frequency generator, the keyboard reader and finish this program
    clk.stop();

    libevdev_free(dev);
    close(fd);

    system("stty echo");

    return EXIT_SUCCESS;
}