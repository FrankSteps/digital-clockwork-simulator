#include "audio_output.hpp"
#include <AudioToolbox/AudioToolbox.h>
#include <stdexcept>

// AudioQueue is callback-driven, so the handle stores the queue and the callback pulls
// from soundBuffer/silenceBuffer directly based on the atomic state
static void audioCallback(void* userData, AudioQueueRef queue, AudioQueueBufferRef bufferRef){
    Buzzer* buzzer = static_cast<Buzzer*>(userData);
    buzzer->fillBuffer(bufferRef);

    AudioQueueEnqueueBuffer(queue, bufferRef, 0, nullptr);
}

void Buzzer::fillBuffer(void* bufferRefVoid){
    AudioQueueBufferRef bufferRef = static_cast<AudioQueueBufferRef>(bufferRefVoid);

    short* dst = static_cast<short*>(bufferRef->mAudioData);
    std::vector<short>& src = state ? soundBuffer : silenceBuffer;

    for(int i = 0; i < bufferSize; i++){
        dst[i] = src[i];
    }

    bufferRef->mAudioDataByteSize = bufferSize * sizeof(short);
}

Buzzer::Buzzer(double toneFrequency) {
    AudioStreamBasicDescription format;
    format.mSampleRate = sampleRate;
    format.mFormatID = kAudioFormatLinearPCM;
    format.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    format.mBytesPerPacket = sizeof(short);
    format.mFramesPerPacket = 1;
    format.mBytesPerFrame = sizeof(short);
    format.mChannelsPerFrame = 1;
    format.mBitsPerChannel = 16;
    format.mReserved = 0;

    AudioQueueRef queue = nullptr;

    if(AudioQueueNewOutput(&format, audioCallback, this, nullptr, kCFRunLoopCommonModes, 0, &queue) != noErr){
        throw std::runtime_error("Buzzer error: could not open audio device.");
    }

    handle = queue;

    tonePeriod = (int)(sampleRate / toneFrequency);
    bufferSize = tonePeriod * 10;

    soundBuffer.resize(bufferSize);
    silenceBuffer.resize(bufferSize, 0);

    for(int i = 0; i < bufferSize; i++){
        soundBuffer[i] = (i % tonePeriod < tonePeriod / 2) ? 32767 : -32767;
    }

    for(int i = 0; i < 3; i++){
        AudioQueueBufferRef bufferRef = nullptr;
        AudioQueueAllocateBuffer(queue, bufferSize * sizeof(short), &bufferRef);
        fillBuffer(bufferRef);
        AudioQueueEnqueueBuffer(queue, bufferRef, 0, nullptr);
    }
}

Buzzer::~Buzzer() {
    stop();

    if(handle){
        AudioQueueDispose(static_cast<AudioQueueRef>(handle), true);
    }
}

void Buzzer::run() {
    AudioQueueRef queue = static_cast<AudioQueueRef>(handle);

    while(running){
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.05, true);
    }

    AudioQueueStop(queue, true);
}

void Buzzer::setState(bool value) {
    state = value;
}

void Buzzer::start() {
    running = true;

    AudioQueueStart(static_cast<AudioQueueRef>(handle), nullptr);
    buzzerThread = std::thread([this](){ run(); });
}

void Buzzer::stop() {
    running = false;

    if(buzzerThread.joinable()){
        buzzerThread.join();
    }
}