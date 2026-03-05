#include "chips.hpp"
#include <iostream>

/*
    Métodos da classe Chip4017
*/
Chip4017::Chip4017(unsigned limitReset) : LimitReset(limitReset) {
    if (LimitReset < 1 || LimitReset > 10) {
        throw std::invalid_argument("LimitReset precisa estar entre 1 e 10.");
    }
    reset();
}

void Chip4017::shift() {
    Out >>= 1;
    if (Out == 0) {
        Out = 1u << (LimitReset - 1);
    }
}

void Chip4017::reset(){
    Out = 1u << (LimitReset - 1);
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

void Chip4081::setInputA(int index, bool value){
    if(index < 0 || index >= 4){
        throw std::invalid_argument("Erro em inputA: Index precisa estar entre 0 e 4.");
    }
    input_A[index] = value;
    updateOutputs();
}

void Chip4081::setInputB(int index, bool value){
    if(index < 0 || index >= 4){
        throw std::invalid_argument("Erro em inputB: Index precisa estar entre 0 e 4.");
    }
    input_B[index] = value;
    updateOutputs();
}

bool Chip4081::getOutput(int index) const{
    if(index < 0 || index > 4){
        throw std::invalid_argument("Erro em output: Index precisa estar entre 0 e 4.");
    }
    return output_C[index];
}