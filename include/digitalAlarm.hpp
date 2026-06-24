#ifndef DIGITALALARM_HPP
#define DIGITALALARM_HPP

#include "chips.hpp"
#include <array>
#include <string>

// DigitalAlarm board class
class DigitalAlarm {
    private:
        Chip4013 cd4013[9];                                                 // flip-flops D — stores programmed time (17 bits) + disarm
        Chip4063 cd4063[6];                                                 // magnitude comparators — cascaded
        Chip4081 cd4081[2];                                                 // AND gates — combines time and day comparisons
        Chip4017 cd4017;                                                    // day of week indicator
        Chip4071 cd4071;                                                    // OR gates — day comparison
        Chip555  ne555;                                                     // astable multivibrator — alarm signal frequency
    
        bool armed = false;                                                 // alarm armed state
        bool ringing = false;                                               // alarm ringing state
    
        std::array<bool, 7> weekDays{};                                     // simulates the 7 ON/OFF switches (.week file)
    
        void loadWeekFile(const std::string& path);                         // reads .week file and populates weekDays
        void updateComparators();                                           // updates cd4063, cd4081, cd4071 state
        bool checkDay();                                                    // "programmed day == current day?"
        bool checkTime();                                                   // "programmed time == current time?"
        void loadMemory();                                                  // applies clock pulse to flip-flops
        void resetMemory();                                                 // resets all flip-flops
    
    public:
        DigitalAlarm();
    
        void programAlarm();                                                // Key_P: programs the alarm to the current time shown on the display
        void disarm();                                                      // Key_D: disarms the alarm while it is ringing
        void advanceDay();                                                  // Key_A: advances the day on cd4017
        void reset();                                                       // Key_R: applies reset to all flip-flops
    
        void setDataMemory(std::array<std::array<bool, 4>, 4> busData);     // receives current time from DigitalClockwork
        bool weekDaysStatus(size_t index) const;                            // returns the state of a given weekday switch
        bool getOutputSystem() const;                                       // returns true when alarm should ring
};

#endif