#ifndef DIGITALALARM_HPP
#define DIGITALALARM_HPP

#include "chips.hpp"
#include "digitalClockwork.hpp"

class DigitalAlarm{
    private:
        std::array<Chip4013*, 9> cd4013;     //  memory
        std::array<Chip4063*, 4> cd4063;     //  comparators
        std::array<Chip4081*, 2> cd4081;
        Chip4017* cd4017;

    public:
        DigitalAlarm(
            std::array<Chip4013*, 9> chips4013,
            std::array<Chip4063*, 4> chips4063,
            std::array<Chip4081*, 2> chips4081,
            Chip4017* chip4017
        );
};

#endif