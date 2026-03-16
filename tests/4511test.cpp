#include "chips.hpp"
#include <iostream>
#include <cstdlib>

int main(){
    Chip4511 chip1;

    chip1.setLampTest(true);
    std::cout << chip1.getSegmentsOut(1) << std::endl;

    return 0;
}