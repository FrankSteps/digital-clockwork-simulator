#ifndef DIGITALALARM_HPP
#define DIGITALALARM_HPP

#include "chips.hpp"
#include <array>
#include <string>

// DigitalAlarm board class
class DigitalAlarm {
    private:
        Chip4013 cd4013[9];                                                                                 // flip-flops D — stores programmed time (17 bits HH:MM + Meridiem) + disarm
        Chip4063 cd4063[5];                                                                                 // magnitude comparators — cascaded
        Chip4081 cd4081[2];                                                                                 // AND gates — combines time and day comparisons
        Chip4071 cd4071[2];                                                                                 // OR gates — day comparison
        Chip4017 cd4017;                                                                                    // day of week indicator

        //bool armed = false;                                                                               // alarm armed state
        //bool ringing = false;                                                                             // alarm ringing state
        bool meridiem = false;                  

        std::array<bool, 7> weekDays{};                                                                     // simulates the 7 ON/OFF switches (.week file)
    
        void loadWeekFile(const std::string& path);                                                         // reads .week file and populates weekDays
        void updateTimeComparators(std::array<std::array<bool, 4>, 4> busData, bool currentMeridiem);       // updates cd4063 cascade with current time and meridiem
        void updateComparators(std::array<std::array<bool, 4>, 4> busData, bool currentMeridiem);           // updates cd4063, cd4081, cd4071 state
        void updateDayComparators();                                                                        // updates cd4081 AND gates and cd4071 OR cascade with current day
        bool checkDay();                                                                                    // "programmed day == current day?"
        bool checkTime();                                                                                   // "programmed time == current time?"
        void loadMemory();                                                                                  // applies clock pulse to flip-flops
        void resetMemory();                                                                                 // resets all flip-flops

    public:                     
        DigitalAlarm(const std::string& weekFilePath);                   

    void programAlarm();                                                                                    // Key_P: programs the alarm to the current time shown on the display
    //void disarm();                                                                                        // Key_D: disarms the alarm while it is ringing
    void advanceDay();                                                                                      // Key_A: advances the day on cd4017
    void reset();                                                                                           // Key_R: applies reset to all flip-flops
        
    void setMeridiem(bool meridien);                                                                        // receives current meridiem state from DigitalClockwork                      
    void setDataMemory(std::array<std::array<bool, 4>, 4> busData);                                         // receives current time from DigitalClockwork
                
    bool getWeekDaysStatus(size_t index) const;                                                             // returns the state of a given weekday switch
    bool getCurrentDay(size_t index) const;                                                                 // returns the current active day output from cd4017
    bool getOutputSystem() const;                                                                           // returns true when alarm should ring
};

#endif