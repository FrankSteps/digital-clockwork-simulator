#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <array>
#include <string>
#include <iostream>

// Seven-segments display
class Display{
    private:
        int decodeDigit(const std::array<bool, 7>& segments);

    public:
        Display() = default;
        std::string render(const std::array<std::array<bool,7>,4>& segments);
};


// Led
class Led{
    private:
        bool state  = false;

    public:
        Led() = default;

        void setState(bool value);
        std::string getState();
};


#endif 