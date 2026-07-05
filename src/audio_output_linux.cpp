#include "audio_output.hpp"
#include <alsa/asoundlib.h>
#include <stdexcept>


/*
    Buzzer
*/

Buzzer::Buzzer(double toneFrequency) {
    snd_pcm_t* pcm = nullptr;

    if(snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0){
        throw std::runtime_error("Buzzer error: could not open audio device.");
    }

    snd_pcm_set_params(pcm,
        SND_PCM_FORMAT_S16_LE,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        1,
        sampleRate,
        1,
        50000
    );

    handle = pcm;

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
        snd_pcm_close(static_cast<snd_pcm_t*>(handle));
    }
}

void Buzzer::run() {
    snd_pcm_t* pcm = static_cast<snd_pcm_t*>(handle);

    while(running){
        if(state){
            snd_pcm_writei(pcm, soundBuffer.data(), bufferSize);
        } else {
            snd_pcm_writei(pcm, silenceBuffer.data(), bufferSize);
        }
    }
    snd_pcm_drain(pcm);
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