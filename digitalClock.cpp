/*
    Esta parte do código eu escrevi apenas para testar a classe Chip4017 reformulada.
    A diferença do Chip4017 que está neste projeto para os demais é que neste eu busquei 
    emular o chip da melhor maneira possível e farei isso com os demais.
    
    Confira o arquivo "chips.hpp" e "chips.cpp"
*/

#include <iostream>
#include <bitset>
#include "chips.hpp"

int main() {
    // apenas Q0 e Q1 ativos para este teste
    const int Qreset = 2;
    Chip4017 chip(Qreset);

    bool clockLevel = false;
    char input;

    while (true) {
        std::cout << std::bitset<Qreset>(chip.getOut()) << "  ";
        input = std::cin.get();

        if (input == 'q') {
            break;
        }
        if (input == 'r') {
            chip.reset();
        }
        if (input == '\n') {
            clockLevel = !clockLevel;
            chip.clock(clockLevel);
        }
    }
    return 0;
}