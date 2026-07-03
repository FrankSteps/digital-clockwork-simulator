#ifndef KEYSTATE_HPP
#define KEYSTATE_HPP

// current state of each key tracked by the program, filled by the platform-specific poll function
struct KeyState {
    bool F;
    bool S;
    bool P;
    bool A;
    bool R;
    bool D;
};

#endif