#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "freqGenerator.hpp"


void FreqGenerator::run(){
    while(running){
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(period_ms/2)));
        {
            std::lock_guard<std::mutex> lock(mtx);
            state = !state;
        }
        condv.notify_all();
    }
}

void FreqGenerator::waitEdge(bool prevState){
    std::unique_lock<std::mutex> lock(mtx);
    condv.wait(lock, [&]{
        return state != prevState;
    });
}

void FreqGenerator::start(){
    running = true;
    clkThread = std::thread(&FreqGenerator::run, this);
}

void FreqGenerator::stop(){
    running = false;
    if(clkThread.joinable()){
        clkThread.join();
    }
}

bool FreqGenerator::getState() const{
    std::lock_guard<std::mutex> lock(mtx);
    return state.load();
}



