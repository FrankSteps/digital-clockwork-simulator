#include "audio_output.hpp"
#include <windows.h>
#include <stdexcept>

Buzzer::Buzzer(double toneFrequency) {
    HWAVEOUT hwo = nullptr;

    WAVEFORMATEX format;
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 1;
    format.nSamplesPerSec = sampleRate;
    format.wBitsPerSample = 16;
    format.nBlockAlign = (format.nChannels * format.wBitsPerSample) / 8;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    format.cbSize = 0;

    if(waveOutOpen(&hwo, WAVE_MAPPER, &format, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR){
        throw std::runtime_error("Buzzer error: could not open audio device.");
    }

    handle = hwo;

    tonePeriod = (int)(sampleRate / toneFrequency);
    bufferSize = tonePeriod * 10;

    soundBuffer.resize(bufferSize);
    silenceBuffer.resize(bufferSize, 0);

    for(int i = 0; i < bufferSize; i++){
        soundBuffer[i] = (i % tonePeriod < tonePeriod / 2) ? 32767 : -32767;
    }
}

Buzzer::~Buzzer() {
    stop();

    if(handle){
        waveOutClose(static_cast<HWAVEOUT>(handle));
    }
}

// Writes one buffer and blocks until playback finishes, mirroring the blocking write style used on Linux
static void writeBuffer(HWAVEOUT hwo, std::vector<short>& buffer, int bufferSize){
    WAVEHDR header;
    ZeroMemory(&header, sizeof(WAVEHDR));

    header.lpData = reinterpret_cast<LPSTR>(buffer.data());
    header.dwBufferLength = bufferSize * sizeof(short);

    waveOutPrepareHeader(hwo, &header, sizeof(WAVEHDR));
    waveOutWrite(hwo, &header, sizeof(WAVEHDR));

    while(!(header.dwFlags & WHDR_DONE)){
        Sleep(1);
    }

    waveOutUnprepareHeader(hwo, &header, sizeof(WAVEHDR));
}

void Buzzer::run() {
    HWAVEOUT hwo = static_cast<HWAVEOUT>(handle);

    while(running){
        if(state){
            writeBuffer(hwo, soundBuffer, bufferSize);
        } else {
            writeBuffer(hwo, silenceBuffer, bufferSize);
        }
    }
}

void Buzzer::setState(bool value) {
    state = value;
}

void Buzzer::start() {
    running = true;
    buzzerThread = std::thread([this](){ run(); });
}

void Buzzer::stop() {
    running = false;

    if(buzzerThread.joinable()){
        buzzerThread.join();
    }
}