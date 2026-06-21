#include "feedback.hpp"
#include <iostream>


/*
    DISPLAY
*/

// Decodes a 7-segment pattern into its corresponding digit (0–9)
int Display::decodeDigit(const std::array<bool, 7>& segments){
    
    if  (segments == std::array<bool,7>  {1,1,1,1,1,1,0})  return 0;
    if  (segments == std::array<bool,7>  {0,1,1,0,0,0,0})  return 1;
    if  (segments == std::array<bool,7>  {1,1,0,1,1,0,1})  return 2;
    if  (segments == std::array<bool,7>  {1,1,1,1,0,0,1})  return 3;
    if  (segments == std::array<bool,7>  {0,1,1,0,0,1,1})  return 4;
    if  (segments == std::array<bool,7>  {1,0,1,1,0,1,1})  return 5;
    if  (segments == std::array<bool,7>  {1,0,1,1,1,1,1})  return 6;
    if  (segments == std::array<bool,7>  {1,1,1,0,0,0,0})  return 7;
    if  (segments == std::array<bool,7>  {1,1,1,1,1,1,1})  return 8;
    if  (segments == std::array<bool,7>  {1,1,1,1,0,1,1})  return 9;

    return -1;
}

// Converts 4 seven-segment digits into a string (HH:MM)
std::string Display::render(const std::array<std::array<bool,7>,4>& segments){
    std::string output;

    for(int i = 3; i >= 0; i--){
        int num = decodeDigit(segments[i]);
        output += std::to_string(num);
    
        if(i == 2){
            output += ":"; 
        }
    }

    return output;
}



/*
    LED
*/

// Set the LED state
void Led::setState(bool value){
    state = value;
}

// Returns a character representing the LED state
std::string Led::getState(){
    if(state){
        return "◉";
    }

    return "◎";
}



/*
    Buzzer
*/

Buzzer::Buzzer(double toneFrequency) {
    if(snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0){
        throw std::runtime_error("Buzzer error: could not open audio device.");
    }

    snd_pcm_set_params(handle,
        SND_PCM_FORMAT_S16_LE,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        1,
        sampleRate,
        1,
        50000
    );

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
        snd_pcm_close(handle);
    }
}

void Buzzer::run() {
    while(running){
        if(state){
            snd_pcm_writei(handle, soundBuffer.data(), bufferSize);
        } else {
            snd_pcm_writei(handle, silenceBuffer.data(), bufferSize);
        }
    }
    snd_pcm_drain(handle);
}

void Buzzer::setState(bool value) {
    state = value;
}

void Buzzer::start() {
    running = true;

    buzzerThread = std::thread([this](){ 
        run(); 
    });
}

void Buzzer::stop() {
    running = false;

    if(buzzerThread.joinable()){
        buzzerThread.join();
    }
}