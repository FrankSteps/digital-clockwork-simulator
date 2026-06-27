/*
[PT-BR]
    Teste do buzzer com ALSA e Chip555. O 555 oscila continuamente e o buzzer
    toca enquanto o chip estiver rodando. Pressione ENTER para parar.

[EN-US]
    Buzzer test with ALSA and Chip555. The 555 oscillates continuously and the
    buzzer plays while the chip is running. Press ENTER to stop.
*/

// libraries
#include "chips.hpp"
#include "feedback.hpp"
#include <iostream>
#include <thread>
#include <atomic>

int main() {
    // ~1Hz: R1=1kΩ, R2=100kΩ, C=6.8µF
    Chip555 timer(1000, 100000, 0.0000068);
    Buzzer buzzer;

    std::cout << "Frequency: " << timer.getFrequency() << " Hz\n";
    std::cout << "Press ENTER to stop...\n";

    std::atomic<bool> running{true};

    timer.start();
    buzzer.start();

    bool lastState = timer.getState();

    std::thread timerThread([&](){
        while(running){
            timer.waitEdge(lastState);
            lastState = timer.getState();
            buzzer.setState(lastState);
        }
    });

    std::cin.get();

    running = false;
    timer.stop();
    buzzer.stop();
    timerThread.join();

    return 0;
}