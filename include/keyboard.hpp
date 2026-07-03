#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "keyState.hpp"

// interface implemented once per platform: keyboard_linux.cpp, keyboard_windows.cpp, keyboard_macos.cpp
// only one of these three .cpp files is compiled at a time, selected by the Makefile

// opens whatever keyboard capture mechanism the platform uses
void initKeyboard();

// returns the current state of the tracked keys
KeyState pollKeys();

// closes the keyboard capture mechanism
void closeKeyboard();

// terminal configuration, called once before the main loop
void terminalSetup();

// terminal restoration, called once after the main loop
void terminalTeardown();

#endif