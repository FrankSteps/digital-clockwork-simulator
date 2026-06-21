#ifndef CHIPS_HPP
#define CHIPS_HPP

// libraries
#include <cstdint>
#include <array>
#include <stdexcept>

#include <condition_variable>
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>

// Johnson counter chip class
class Chip4017 {
    private:
        std::array <bool, 10> outputs{0,0,0,0,0,0,0,0,0,0};

        unsigned LimitReset;
        bool ClockEnable = false;

        unsigned int value = 0;  

    public:
        explicit Chip4017(unsigned limitReset = 10, bool clockEnable = false);

        void shift();
        void reset();

        bool getOutput(size_t index) const;
        unsigned getLimitReset() const;
};


// AND gates chip class
class Chip4081 {
    private:
        std::array<bool, 4> input_A  = {0,0,0,0};
        std::array<bool, 4> input_B  = {0,0,0,0};
        std::array<bool, 4> output_C = {0,0,0,0};
        
        void updateOutputs();

    public:
        Chip4081() = default;

        void setInputA(size_t index, bool value);
        void setInputB(size_t index, bool value);
        bool getOutput(size_t index) const;
};


// BCD counters chip class
class Chip4029 {
    private:
        std::array<bool,4> presetInputs{0,0,0,0};
        std::array<bool,4> outputs{0,0,0,0};

        const std::array<bool,4> ZERO{0,0,0,0};
        const std::array<bool,4> NINE{1,0,0,1};
        const std::array<bool,4> FIFTEEN{1,1,1,1};

        bool presetEnable = false;
        bool carryIn = false;
        bool upDown = false;
        bool binaryDecade = false;
        bool carryOut = false;

        int value = 0;

        void increment();
        void decrement();

    public:
        explicit Chip4029(const std::array<bool,4>& presetInputs){
            if(presetInputs.size() != 4){ 
                throw std::invalid_argument("presetInputs requires 4 valid boolean values");
            }
            this->presetInputs = presetInputs;
            this->outputs = presetInputs;
        }

        void setPresetEnable();
        void setCarryIn(bool value);
        void setUpDown(bool value);
        void setBinaryDecade(bool value);
        bool getCarryOut() const;
        bool getOutput(size_t index) const;

        void clock();
};


// seven-segment decoder chip class
class Chip4511{
    private:
        std::array<bool, 7> segmentsOut{0,0,0,0,0,0,0};
        std::array<bool, 4> inputs{0,0,0,0};

        bool lampTest = false;
        bool blanking = false;
        bool latchEnb = false; 

        void updateSegmentsOut();

    public:
        Chip4511() = default;

        void setLampTest(bool value);
        void setBlanking(bool value);
        void setLatchEnb(bool value);
        void setInputs(std::array<bool, 4> index);

        bool getSegmentsOut(size_t index) const;
};


// ripple counter chip class
class Chip4040{
    private:
        std::array<bool, 12> outputs = {0,0,0,0,0,0,0,0,0,0,0,0};
        int value = 0;
        void increment();

    public:
        Chip4040() = default;

        void reset();
        void clock();

        bool getOutput(size_t index) const;
};


// magnitude comparator
class Chip4063 {
    private:
        std::array<bool, 4> input_A{0,0,0,0};
        std::array<bool, 4> input_B{0,0,0,0};


        bool inputEqual   = false,
             inputGreater = false,
             inputSmaller = false;


        bool outputEqual   = false,
             outputGreater = false,
             outputSmaller = false;


        void updateOutputs();

    public:
        Chip4063() = default;

        void setInputA(size_t index, bool value);
        void setInputB(size_t index, bool value);

        void setInputEqual(bool value);
        void setInputGreater(bool value);
        void setInputSmaller(bool value);

        bool getOutputEqual() const;
        bool getOutputGreater() const;
        bool getOutputSmaller() const;
};


// Flip-Flops D
class Chip4013 {
    private:
        std::array<bool, 2> output;                  // Q
        std::array<bool, 2> negOutput;               // ~Q
        std::array<bool, 2> data;                    // D

        void updateOutputs(size_t flipflop);

    public:
        explicit Chip4013();

        void setData(size_t flipflop, bool value);
        void set(size_t flipflop);

        void reset(size_t flipflop);
        void clock(size_t flipflop);

        bool getOutput(size_t index) const;
        bool getNegOutput(size_t index) const;
};


// astable multivibrator chip class
class Chip555 {
    private:
        double R1, R2, C;

        double tHigh  = 0.0;
        double tLow   = 0.0;
        double period = 0.0;
        double freq   = 0.0;

        const double Ln2 = 0.693;

        std::atomic<bool> running{false};
        std::atomic<bool> state{false};

        std::thread clkThread;
        std::condition_variable condv;
        mutable std::mutex mtx;

        void calcTimings();                         // calculates tHigh, tLow, period and frequency from R1, R2 and C
        void run();                                 // thread loop: toggles state at tHigh and tLow intervals

    public:
        explicit Chip555(double r1Ohms, double r2Ohms, double cFarads);

        void start();                               // starts the oscillator thread
        void stop();                                // stops the oscillator thread and waits for it to finish
        void waitEdge(bool prevState);              // blocks until the signal changes state (edge detection)
        bool getState() const;                      // returns the current signal state (thread-safe)
        double getFrequency() const;                // returns the oscillation frequency in Hz
        double getPeriod() const;                   // returns the full period in seconds
        double getTHigh() const;                    // returns the time of high state
        double getTLow() const;                     // returns the time of low state
};

#endif