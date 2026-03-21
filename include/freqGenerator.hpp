#ifndef FREQGENERATOR_HPP
#define FREQGENERATOR_HPP

#include <condition_variable>
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>

class FreqGenerator{
    private:
        double period_ms = 1000.0 / 60;
        std::atomic<bool> running; 
        std::atomic<bool> state;
        std::thread clkThread;

        std::condition_variable condv;

        // Mutex is mutable so it can be used in const methods without affecting the object's logical state.
        mutable std::mutex mtx;

        void run();

    public:
        FreqGenerator() = default;

        void waitEdge(bool prevState);
        void start();
        void stop();

        bool getState() const;
};

#endif