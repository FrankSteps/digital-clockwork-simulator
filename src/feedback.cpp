#include "feedback.hpp"
#include <iostream>


/*

    Windows console defaults to a legacy code page (CP850/CP437), which garbles the UTF-8 LED symbols (◉/◎) printed by Display. 
    This runs before main() starts, via static initialization, forcing the console to interpret and output UTF-8 correctly. 
    No-op on Linux/macOS, where the terminal already handles UTF-8 natively.

*/

#ifdef _WIN32
#include <windows.h>

namespace {
    struct ConsoleUTF8Setup {
        ConsoleUTF8Setup() {
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
        }
    };

    ConsoleUTF8Setup consoleUTF8Setup;
}
#endif


/*
    DISPLAY
*/

// Decodes a 7-segment pattern into its corresponding digit (0–9)
int Display::decodeDigit(const std::array<bool, 7>& segments){
    
    if  (segments == std::array<bool,7>  {1,1,1,1,1,1,0})  return 0;
    if  (segments == std::array<bool,7>  {0,1,1,0,0,0,0})  return 1;
    if  (segments == std::array<bool,7>  {1,1,0,1,1,0,1})  return 2;
    if  (segments == std::array<bool,7>  {1,1,1,1,0,0,1})  return 3;
    if  (segments == std::array<bool,7>  {0,1,1,0,0,1,1})  return 4;
    if  (segments == std::array<bool,7>  {1,0,1,1,0,1,1})  return 5;
    if  (segments == std::array<bool,7>  {1,0,1,1,1,1,1})  return 6;
    if  (segments == std::array<bool,7>  {1,1,1,0,0,0,0})  return 7;
    if  (segments == std::array<bool,7>  {1,1,1,1,1,1,1})  return 8;
    if  (segments == std::array<bool,7>  {1,1,1,1,0,1,1})  return 9;

    return -1;
}

// Converts 4 seven-segment digits into a string (HH:MM)
std::string Display::render(const std::array<std::array<bool,7>,4>& segments){
    std::string output;

    for(int i = 3; i >= 0; i--){
        int num = decodeDigit(segments[i]);
        output += std::to_string(num);
    
        if(i == 2){
            output += ":"; 
        }
    }

    return output;
}



/*
    LED
*/

// Set the LED state
void Led::setState(bool value){
    state = value;
}

// Returns a character representing the LED state
std::string Led::getState(){
    if(state){
        return "◉";
    }

    return "◎";
}