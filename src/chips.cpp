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
    if(ClockEnable){
        Out >>= 1;
        if (Out == 0) {
            Out = 1u << (LimitReset - 1);
        }
    }
}

void Chip4017::reset(){
    Out = 1;
}

uint32_t Chip4017::getOut() const{
    return Out;
}

unsigned Chip4017::getLimitReset() const{
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
    if(index < 0 || index >= 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    input_A[index] = value;
    updateOutputs();
}

void Chip4081::setInputB(size_t index, bool value){
    if(index < 0 || index >= 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    input_B[index] = value;
    updateOutputs();
}

bool Chip4081::getOutput(size_t index) const{
    if(index < 0 || index > 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    return output_C[index];
}


/*
    Métodos da classe Chip4029
*/



bool Chip4029::getOutput(size_t index) const{
    if(index < 0 || index > 4){
        throw std::invalid_argument("Output error: output index out of range. Valid indices are 0 to 3.");
    }
    return outputs[index];
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