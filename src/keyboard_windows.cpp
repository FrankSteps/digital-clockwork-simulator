/*
[WINDOWS]
    Reads keyboard state via GetAsyncKeyState (polling, no window required).
*/

#include "keyboard.hpp"

#include <cstdlib>                                        // system()
#include <windows.h>                                      // GetAsyncKeyState


void terminalTeardown(){}   // no equivalent to stty needed
void initKeyboard(){}       // no handle to open: GetAsyncKeyState queries the system directly
void closeKeyboard(){}      // nothing to close


KeyState pollKeys(){
    KeyState state;

    state.F = (GetAsyncKeyState('F') & 0x8000) != 0;
    state.S = (GetAsyncKeyState('S') & 0x8000) != 0;
    state.P = (GetAsyncKeyState('P') & 0x8000) != 0;
    state.A = (GetAsyncKeyState('A') & 0x8000) != 0;
    state.R = (GetAsyncKeyState('R') & 0x8000) != 0;
    state.D = (GetAsyncKeyState('D') & 0x8000) != 0;

    return state;
}



void terminalSetup(){
    system("cls");
}