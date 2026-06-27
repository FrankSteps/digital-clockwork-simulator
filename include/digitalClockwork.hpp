#ifndef DIGITALCLOCKWORK_HPP
#define DIGITALCLOCKWORK_HPP

#include "chips.hpp"
#include <array>


// DigitalClockwork board class 
class DigitalClockwork{
    // Enum class placed here so it can be accessed by all encapsulated methods
    public:
        enum class ADJUSTMENT{                          
            FAST,                                       // Press 'F' to select Fast mode 
            SLOW,                                       // Press 'S' to select Slow mode 
            DEFAULT                                     // Default .-.
        };

    private:
        Chip4029 cd4029[4] = {
            Chip4029({0,0,0,0}),
            Chip4029({0,0,0,0}),
            Chip4029({1,0,0,0}),
            Chip4029({0,0,0,0})
        };

        Chip4511 cd4511[4];
        Chip4081 cd4081[2];
        Chip4017 cd4017;
        Chip4040 cd4040;

        
        // Edge detection flags for frequency conversion
        bool lastQ0 = false;    
        bool lastQ5 = false;           
        bool lastOut4081 = false; 
        bool meridienEdge = false;

        // contador para a gambiarra <- eu sei que eu preciso resolver isso aqui, mas estou com tanta preguiça. Imagina eu dou uma de Fermat e nunca resolvo isso aqui!? O pior é que eu tenho uma noção de resolver kk
        int defaultCooldown = 0;


        void cascadeCount();                             // Simulates the ripple carry between the minute counters
        void updateAND();                                // Updates all four AND gates of cd4081[0]
        void applyResets();                              // Applies resets and special clock pulses based on AND gate outputs
        void updateDisplays();                           // updating the seven-segment display state
        void updateCounter();                            // Orchestrates the full update cycle in the correct order
        void updateAND_1();                              // Updates all four AND gates of cd4081[1]

        /*
            Acquires the clock signal from cd4040 and divides its frequency
            to control the counting speed of the clock
        */
        void frequencyConverter(ADJUSTMENT MODE);


    public:
        DigitalClockwork();

        // Advances the system clock and updates the circuit based on the selected mode
        void updateSystem(ADJUSTMENT MODE = ADJUSTMENT::DEFAULT);

        // Returns the current seven-segment output state of all four display digits
        std::array<std::array<bool, 7>, 4> getSegmentsOutput();

        // Returns the current output state of all four counters
        std::array<std::array<bool, 4>, 4> getCountersOutput();

        // AM or PM?
        bool getMeridien(size_t value);

        //
        bool get4040FrequencyQ6();
};

#endif