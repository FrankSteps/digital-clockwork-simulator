#ifndef BUZZER_HPP
#define BUZZER_HPP

#include <atomic>
#include <thread>
#include <vector>


// Buzzer
class Buzzer {
    private:
        void* handle = nullptr;

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

        void fillBuffer(void* bufferRef); // used only in the macOS implementation
};

#endif