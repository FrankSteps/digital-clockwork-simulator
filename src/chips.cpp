#include "chips.hpp"

#include <condition_variable>
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>


/*
    Chip4017
*/

Chip4017::Chip4017() {
    value = 1;
    reset();
}

void Chip4017::setReset(unsigned value){
    if(value <= 0 || value > 10){
        throw std::invalid_argument("Chip4017 error: input A index out of range. Valid indices are 1 to 10.");
    }
    LimitReset = value;
}

void Chip4017::setClockEnable(bool value){
    ClockEnable = value;
}


void Chip4017::shift() {
    if (!ClockEnable){
        return;
    }

    value <<= 1;

    if (value >= (1u << LimitReset)) {
        value = 1;
    }

    outputs.fill(false);

    for (size_t i = 0; i < 10; i++) {
        outputs[i] = (value >> i) & 1;
    }
}


void Chip4017::reset() {
    outputs = {1,0,0,0,0,0,0,0,0,0};
}


bool Chip4017::getOutput(size_t index) const {
    if(index >= 10){
        throw std::invalid_argument("Chip4017 error: index out of range. Valid indices are 0 to 9.");
    }
    return outputs[index];
}


unsigned Chip4017::getLimitReset() const {
    return LimitReset;
}


/*
    Chip4081
*/

void Chip4081::updateOutputs(){
    for(int i = 0; i < 4; i++){
        output_C[i] = input_A[i] && input_B[i];
    }
}

void Chip4081::setInputA(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Chip4081 error: input A index out of range. Valid indices are 0 to 3.");
    }
    input_A[index] = value;
    updateOutputs();
}

void Chip4081::setInputB(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Chip4081 error: input B index out of range. Valid indices are 0 to 3.");
    }
    input_B[index] = value;
    updateOutputs();
}

bool Chip4081::getOutput(size_t index) const{
    if(index >= 4){
        throw std::invalid_argument("Chip4081 error: output index out of range. Valid indices are 0 to 3.");
    }
    return output_C[index];
}



/*
    Chip4029
*/

bool Chip4029::getOutput(size_t index) const{
    if(index >= 4){
        throw std::invalid_argument("Chip4029 error: output index out of range. Valid indices are 0 to 3.");
    }
    return outputs[index];
}


void Chip4029::increment(){
    if (!carryIn) {
        carryOut = false;
        return;
    }

    value = 0;
    for (size_t i = 0; i < 4; i++) {
        if (outputs[i]) value |= (1 << i);
    }

    value += 1;

    if (!binaryDecade) {
        carryOut = (value > 9);

        if (carryOut) {
            value %= 10;
        }

    } else {
        carryOut = (value > 15);

        if (carryOut) {
            value %= 16;
        }
    }

    for (size_t i = 0; i < 4; i++) {
        outputs[i] = (value >> i) & 1;
    }
}


void Chip4029::decrement(){
    if (!carryIn){
        carryOut = false;
        return;
    }

    value = 0;
    for (size_t i = 0; i < 4; i++) {
        if (outputs[i]){
            value |= (1 << i);
        }
    }

    value -= 1;

    if (!binaryDecade) {
        carryOut = (value < 0);
        if (carryOut){
            value += 10;
        } 
    } else {
        carryOut = (value < 0);
        if (carryOut) {
            value += 16;
        }
    }

    for (size_t i = 0; i < 4; i++) {
        outputs[i] = (value >> i) & 1;
    }
}



bool Chip4029::getCarryOut() const{
    return carryOut;
}

void Chip4029::setCarryIn(bool value) {
    carryIn = value;
}

void Chip4029::setPresetEnable(){
    presetEnable = true;
    carryOut = false; 
    outputs = presetInputs;
}

void Chip4029::setBinaryDecade(bool value){  
    binaryDecade = value;
}

void Chip4029::setUpDown(bool value){
    upDown = value;
}

void Chip4029::clock() {
    if (presetEnable) {
        outputs = presetInputs;
        presetEnable = false;
        return;
    }

    if (!carryIn) {
        carryOut = false;
        return;
    }

    if (upDown){
        increment();
    } 
    else {
        decrement();
    }
}


/*
    Chip4511
*/

void Chip4511::setLampTest(bool value){
    lampTest = value;
    updateSegmentsOut();
}

void Chip4511::setBlanking(bool value){
    blanking = value;
    updateSegmentsOut();
}

void Chip4511::setLatchEnb(bool value){
    latchEnb = value;
    updateSegmentsOut();
}

void Chip4511::setInputs(std::array<bool,4> setInputs){
    if(!latchEnb){
        inputs = setInputs;
        updateSegmentsOut();
    }
}

void Chip4511::updateSegmentsOut(){
    if(!lampTest){
        segmentsOut.fill(1);
        return;
    }

    if(!blanking){
        segmentsOut.fill(0);
        return;
    }

    int value = 0;
    for (size_t i = 0; i < 4; i++) {
        if (inputs[i]) {
            value |= (1 << i);
        }
    }

    /*
     Display: 
        a 
      f   b
        g
      e   c
        d   

    segmentsOut = {a, b, c, d, e, f, g};
    */

    static const std::array<std::array<bool, 7>, 10> output{{
        {1,1,1,1,1,1,0},     // 0
        {0,1,1,0,0,0,0},     // 1
        {1,1,0,1,1,0,1},     // 2
        {1,1,1,1,0,0,1},     // 3
        {0,1,1,0,0,1,1},     // 4 
        {1,0,1,1,0,1,1},     // 5
        {1,0,1,1,1,1,1},     // 6
        {1,1,1,0,0,0,0},     // 7
        {1,1,1,1,1,1,1},     // 8
        {1,1,1,1,0,1,1}      // 9
    }};

    if(value < 10){
        segmentsOut = output[value];
    }else{
        segmentsOut.fill(0);
    }
}


bool Chip4511::getSegmentsOut(size_t index) const{
    if(index >= segmentsOut.size()){
        return false;
    }
    return segmentsOut[index];
}



/*
    Chip4040
*/

void Chip4040::reset(){
    outputs.fill(0);
}

void Chip4040::increment(){
    value = 0;
    for(size_t i = 0; i < 12; i++){
        if(outputs[i]){
            value |= (1 << i);
        }
    }

    value = (value + 1) & 0xFFF;
    for(size_t i = 0; i < 12; i++){
        outputs[i] = (value >> i) & 1;
    }
}

void Chip4040::clock(){
    increment();
}

bool Chip4040::getOutput(size_t index) const{
    return outputs[index];
}



/*
    Chip4063
*/

void Chip4063::updateOutputs() {
    for(int i = 3; i >= 0; i--){
        if(input_A[i] && !input_B[i]){
            outputGreater = true;
            outputSmaller = false;
            outputEqual   = false;
            return;
        }
        if(!input_A[i] && input_B[i]){
            outputGreater = false;
            outputSmaller = true;
            outputEqual   = false;
            return;
        }
    }
    outputGreater = inputGreater;
    outputSmaller = inputSmaller;
    outputEqual   = inputEqual;
}


void Chip4063::setInputA(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Chip4063 error: output index out of range. Valid indices are 0 to 3.");
    }
    input_A[index] = value;
    updateOutputs();
}


void Chip4063::setInputB(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Chip4063 error: output index out of range. Valid indices are 0 to 3.");
    }
    input_B[index] = value;
    updateOutputs();
}



void Chip4063::setInputEqual(bool value){
    inputEqual = value;
    updateOutputs();
}

void Chip4063::setInputGreater(bool value){
    inputGreater = value;
    updateOutputs();
}

void Chip4063::setInputSmaller(bool value){
    inputSmaller = value;
    updateOutputs();
}



bool Chip4063::getOutputEqual() const{
    return outputEqual;
}

bool Chip4063::getOutputGreater() const{
    return outputGreater;
}

bool Chip4063::getOutputSmaller() const{
    return outputSmaller;
}


/*
    Chip4013
*/

void Chip4013::updateOutputs(size_t flipflop){
    negOutput[flipflop] = !output[flipflop];
}


Chip4013::Chip4013() {
    output.fill(false);
    negOutput.fill(true);
    data.fill(false);
}

void Chip4013::setData(size_t flipflop, bool value){
    if(flipflop >= 2){
        throw std::invalid_argument("Chip4013 error: flipflop index out of range. Valid indices are 0 to 1.");
    }
    data[flipflop] = value;
}


void Chip4013::set(size_t flipflop){
    if(flipflop >= 2){
        throw std::invalid_argument("Chip4013 error: flipflop index out of range. Valid indices are 0 to 1.");
    }
    output[flipflop] = true;
    updateOutputs(flipflop);
}

void Chip4013::reset(size_t flipflop){
    if(flipflop >= 2){
        throw std::invalid_argument("Chip4013 error: flipflop index out of range. Valid indices are 0 to 1.");
    }
    output[flipflop] = false;
    updateOutputs(flipflop);
}

void Chip4013::clock(size_t flipflop){
    if(flipflop >= 2){
        throw std::invalid_argument("Chip4013 error: flipflop index out of range. Valid indices are 0 to 1.");
    }
    output[flipflop] = data[flipflop];
    updateOutputs(flipflop);
}


bool Chip4013::getOutput(size_t index) const{
    if(index >= 2){
        throw std::invalid_argument("Chip4013 error: flipflop index out of range. Valid indices are 0 to 1.");
    }
    return output[index];
}

bool Chip4013::getNegOutput(size_t index) const{
    if(index >= 2){
        throw std::invalid_argument("Chip4013 error: flipflop index out of range. Valid indices are 0 to 1.");
    }
    return negOutput[index];
}



/*
    Chip555 in Astable Mode
*/

void Chip555::calcTimings() {
    tHigh  = Ln2 * (R1 + R2) * C;
    tLow   = Ln2 * R2 * C;
    period = tHigh + tLow;
    freq   = (period > 0) ? (1.0 / period) : 0.0;
}

/*
    Thread loop: toggles state HIGH for tHigh seconds, then LOW for tLow seconds.
    Uses sleep_until anchored to absolute time points to reduce cumulative drift.
*/
void Chip555::run() {
    std::chrono::steady_clock::time_point next = std::chrono::steady_clock::now();

    std::chrono::steady_clock::duration halfHigh = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<double>(tHigh)
    );

    std::chrono::steady_clock::duration halfLow = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<double>(tLow)
    );

    while(running) {
        next += halfHigh;
        std::this_thread::sleep_until(next);
        {
            std::lock_guard<std::mutex> lock(mtx);
            state = true;
        }
        condv.notify_all();

        next += halfLow;
        std::this_thread::sleep_until(next);
        {
            std::lock_guard<std::mutex> lock(mtx);
            state = false;
        }
        
        condv.notify_all();
    }
}


Chip555::Chip555(double r1Ohms, double r2Ohms, double cFarads) : R1(r1Ohms), R2(r2Ohms), C(cFarads) {
    if(R1 <= 0 || R2 <= 0 || C <= 0) {
        throw std::invalid_argument("Chip555 error: R1, R2 and C must be greater than 0.");
    }
    calcTimings();
}

// starts the oscillator thread
void Chip555::start() {
    running = true;
    
    clkThread = std::thread([this]() {
        run();
    });
}

// stops the oscillator thread and waits for it to finish
void Chip555::stop() {
    running = false;
    condv.notify_all();

    if(clkThread.joinable()){
        clkThread.join();
    }
}

// blocks until the signal changes state (edge detection)
void Chip555::waitEdge(bool prevState) {
    std::unique_lock<std::mutex> lock(mtx);

    condv.wait(lock, [&]{
        return state != prevState;
    });
}

// returns the current signal state (thread-safe)
bool Chip555::getState() const{
    std::lock_guard<std::mutex> lock(mtx);
    return state.load();
}


double Chip555::getFrequency() const{
    return freq;
}


double Chip555::getPeriod() const{
    return period;
}


double Chip555::getTHigh() const{
    return tHigh;
}


double Chip555::getTLow() const{
    return tLow;
}


/*
    Chip4071
*/

void Chip4071::updateOutputs(){
    for(int i = 0; i < 4; i++){
        output_C[i] = input_A[i] || input_B[i];
    }
}


void Chip4071::setInputA(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Chip4071 error: input A index out of range. Valid indices are 0 to 3.");
    }
    input_A[index] = value;
    updateOutputs();
}


void Chip4071::setInputB(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Chip4071 error: input B index out of range. Valid indices are 0 to 3.");
    }
    input_B[index] = value;
    updateOutputs();
}


bool Chip4071::getOutput(size_t index) const{
    if(index >= 4){
        throw std::invalid_argument("Chip4071 error: output index out of range. Valid indices are 0 to 3.");
    }
    return output_C[index];
}