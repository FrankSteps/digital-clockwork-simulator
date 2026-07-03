/*
[LINUX]
    Reads keyboard events via libevdev from the raw kernel device.
*/

#include "keyboard.hpp"

#include <iostream>                                     // std::cerr
#include <cstdlib>                                       // system(), EXIT_FAILURE

#include <libevdev/libevdev.h>                          // keyboard input events
#include <fcntl.h>                                       // open(), O_RDONLY, O_NONBLOCK
#include <unistd.h>                                      // close()


static int fd = -1;
static struct libevdev* dev = nullptr;
static KeyState state = {false, false, false, false, false, false};


void initKeyboard(){
    fd = open("/dev/input/event4", O_RDONLY | O_NONBLOCK);

    if(fd < 0){
        std::cerr << "The file couldn't open\n";
        exit(EXIT_FAILURE);
    }

    libevdev_new_from_fd(fd, &dev);
}


KeyState pollKeys(){
    struct input_event ev;

    if(libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev) == 0){
        if(ev.type == EV_KEY){
            bool pressed = (ev.value == 1);
            bool released = (ev.value == 0);

            if(ev.code == KEY_F && pressed){ state.F = true; } if(ev.code == KEY_F && released){ state.F = false; }
            if(ev.code == KEY_S && pressed){ state.S = true; } if(ev.code == KEY_S && released){ state.S = false; }
            if(ev.code == KEY_P && pressed){ state.P = true; } if(ev.code == KEY_P && released){ state.P = false; }
            if(ev.code == KEY_A && pressed){ state.A = true; } if(ev.code == KEY_A && released){ state.A = false; }
            if(ev.code == KEY_R && pressed){ state.R = true; } if(ev.code == KEY_R && released){ state.R = false; }
            if(ev.code == KEY_D && pressed){ state.D = true; } if(ev.code == KEY_D && released){ state.D = false; }
        }
    }

    return state;
}


void closeKeyboard(){
    libevdev_free(dev);
    close(fd);
}


void terminalSetup(){
    system("clear");
    system("stty -echo");
}


void terminalTeardown(){
    system("stty echo");
}