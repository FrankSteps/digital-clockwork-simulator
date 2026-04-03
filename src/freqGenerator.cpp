#include <chrono>
#include <thread>
#include <mutex>
#include "freqGenerator.hpp"

/*
    Using sleep_until anchors execution to absolute time points. Even if a cycle is delayed, the next iteration 
    compensates, reducing cumulative timing drift
*/
void FreqGenerator::run(){
    std::chrono::steady_clock::time_point next = std::chrono::steady_clock::now();

    std::chrono::steady_clock::duration half = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<double, std::milli>(period_ms / 2)
    );

    while(running){
        next += half;
        std::this_thread::sleep_until(next);
        {
            std::lock_guard<std::mutex> lock(mtx);
            state = !state;
        }
        condv.notify_all();
    }
}

// waits until the signal changes state (edge detection)
void FreqGenerator::waitEdge(bool prevState){
    std::unique_lock<std::mutex> lock(mtx);
    condv.wait(lock, [&]{
        return state != prevState;
    });
}


// starts the frequency generator thread
void FreqGenerator::start(){
    running = true;
    clkThread = std::thread([this]() {
        run();
    });
}


// stops the generator thread and waits for it to finish.
void FreqGenerator::stop(){
    running = false;
    if(clkThread.joinable()){
        clkThread.join();
    }
}

// returns the current signal state (thread-safe)
bool FreqGenerator::getState() const{
    std::lock_guard<std::mutex> lock(mtx);
    return state.load();
}



