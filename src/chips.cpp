#include "chips.hpp"
#include <iostream>



/*
    Métodos da classe Chip4017
*/

Chip4017::Chip4017(unsigned limitReset, bool clockEnable) : LimitReset(limitReset), ClockEnable(clockEnable) {
    if (LimitReset < 1 || LimitReset > 10) {
        throw std::invalid_argument("Chip4017 error: Chip4017 index out of range. Valid indices are 0 to 3.");
    }
    reset();
}

void Chip4017::shift() {
    if (!ClockEnable) {
        return; 
    }

    value = 0;
    for (size_t i = 0; i < 10; i++) {
        if (outputs[i]) {
            value |= (1 << i);
        }
    }
    value <<= 1;

    if (value >= (1u << LimitReset)) {
        value = 1;
    }

    for (size_t i = 0; i < 10; i++) {
        outputs[i] = (value >> i) & 1;
    }
}

void Chip4017::reset() {
    outputs = {1,0,0,0,0,0,0,0,0,0};
}

bool Chip4017::getOutput(size_t index) const {
    return outputs[index];
}

unsigned Chip4017::getLimitReset() const {
    return LimitReset;
}


/*
    Métodos da classe Chip4081
*/

void Chip4081::updateOutputs(){
    for(int i = 0; i < 4; i++){
        output_C[i] = input_A[i] && input_B[i];
    }
}

void Chip4081::setInputA(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    input_A[index] = value;
    updateOutputs();
}

void Chip4081::setInputB(size_t index, bool value){
    if(index >= 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    input_B[index] = value;
    updateOutputs();
}

bool Chip4081::getOutput(size_t index) const{
    if(index > 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    return output_C[index];
}



/*
    Métodos da classe Chip4029
*/

bool Chip4029::getOutput(size_t index) const{
    if(index > 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    return outputs[index];
}


void Chip4029::increment(){
    value = 0;

    for (size_t i = 0; i < 4; i++) {
        if (outputs[i]) {
            value |= (1 << i);
        }
    }

    if (carryIn) {
        value = value + 2;
    } else {
        value = value + 1;
    }

    if (binaryDecade) {
        if (value > 9) {
            value = value % 10;
            carryOut = true;
        } else {
            carryOut = false;
        }
    } else {
        if (value > 15) {
            value = value % 16;
            carryOut = true;
        } else {
            carryOut = false;
        }
    }

    for (size_t i = 0; i < 4; i++) {
        outputs[i] = (value >> i) & 1;
    }
}


void Chip4029::decrement(){
    value = 0;

    for (size_t i = 0; i < 4; i++) {
        if (outputs[i]) {
            value |= (1 << i);
        }
    }

    if (carryIn) {
        value = value - 2;
    } else {
        value = value - 1;
    }

    if (binaryDecade) {
        if (value < 0) {
            value = 10 + value; 
            carryOut = true;
        } else {
            carryOut = false;
        }
    } else {
        if (value < 0) {
            value = 16 + value; 
            carryOut = true;
        } else {
            carryOut = false;
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
    }

    if (upDown){
        decrement();
    }
    else{
        increment();
    }
}

void Chip4029::reset() {
    outputs = presetInputs; 
    carryOut = false;       
}



/*
    Métodos da classe Chip4511
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
    if(lampTest){
        segmentsOut.fill(1);
        return;
    }

    if(blanking){
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
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4 
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    }};

    if(value < 10){
        segmentsOut = output[value];
    }else{
        segmentsOut.fill(0);
    }
}


bool Chip4511::getSegmentsOut(size_t index) const {
    if(index >= segmentsOut.size()){
        return false;
    }
    return segmentsOut[index];
}



/*
    Métodos da classe Chip4040
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