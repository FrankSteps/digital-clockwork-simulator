#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <alsa/asoundlib.h>
#include <atomic>
#include <thread>
#include <vector>

#include <array>
#include <string>
#include <iostream>


// Seven-segments display
class Display{
    private:
        int decodeDigit(const std::array<bool, 7>& segments);

    public:
        Display() = default;
        std::string render(const std::array<std::array<bool,7>,4>& segments);
};


// Led
class Led{
    private:
        bool state  = false;

    public:
        Led() = default;

        void setState(bool value);
        std::string getState();
};


// Buzzer
// Buzzer
class Buzzer {
    private:
        snd_pcm_t* handle = nullptr;

        std::atomic<bool> running{false};
        std::atomic<bool> state{false};

        std::thread buzzerThread;

        int sampleRate = 44100;
        int tonePeriod = 0;
        int bufferSize = 0;

        std::vector<short> soundBuffer;
        std::vector<short> silenceBuffer;

        void run();

    public:
        explicit Buzzer(double toneFrequency = 2000.0);
        ~Buzzer();

        void setState(bool value);
        void start();
        void stop();
};


#endif 