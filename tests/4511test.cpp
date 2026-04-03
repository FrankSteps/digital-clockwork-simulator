/*
[PT-BR]
    Teste individual do chip cd4511

[EN-US]
    Individual test of the chip cd4511

*/

#include "chips.hpp"
#include <iostream>
#include <cstdlib>

int main(){
    Chip4511 chip1;

    chip1.setLampTest(true);
    std::cout << chip1.getSegmentsOut(2) << std::endl;

    return 0;
}