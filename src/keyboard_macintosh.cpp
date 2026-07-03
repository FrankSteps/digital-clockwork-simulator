/*
[MACOS]
    Reads keyboard events via CGEventTap (system-wide, no window required).

    NOTE: requires Accessibility permission granted to the binary in System Settings > Privacy & Security > Accessibility, 
    otherwise the event tap silently receives no events.
*/

#include "keyboard.hpp"

#include <iostream>                                      // std::cerr
#include <cstdlib>                                       // system(), EXIT_FAILURE

#include <ApplicationServices/ApplicationServices.h>     // CGEventTap
#include <Carbon/Carbon.h>                               // kVK_ANSI_* virtual keycodes


#define VK_KEY_F kVK_ANSI_F
#define VK_KEY_S kVK_ANSI_S
#define VK_KEY_P kVK_ANSI_P
#define VK_KEY_A kVK_ANSI_A
#define VK_KEY_R kVK_ANSI_R
#define VK_KEY_D kVK_ANSI_D


static CFMachPortRef tap = nullptr;
static CFRunLoopSourceRef runLoopSource = nullptr;
static KeyState state = {false, false, false, false, false, false};


CGEventRef keyTapCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon){
    if(type == kCGEventKeyDown || type == kCGEventKeyUp){
        CGKeyCode code = static_cast<CGKeyCode>(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
        bool pressed = (type == kCGEventKeyDown);

        if(code == VK_KEY_F){ 
            state.F = pressed; 
        }

        if(code == VK_KEY_S){ 
            state.S = pressed; 
        }

        if(code == VK_KEY_P){ 
            state.P = pressed; 
        }

        if(code == VK_KEY_A){ 
            state.A = pressed; 
        }

        if(code == VK_KEY_R){ 
            state.R = pressed; 
        }

        if(code == VK_KEY_D){ 
            state.D = pressed; 
        }
    }

    return event;
}


void initKeyboard(){
    CGEventMask mask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp);

    tap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionListenOnly, mask, keyTapCallback, nullptr);

    if(tap == nullptr){
        std::cerr << "The event tap couldn't be created (check Accessibility permission)\n";
        exit(EXIT_FAILURE);
    }

    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, tap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(tap, true);
}


KeyState pollKeys(){
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
    return state;
}


void closeKeyboard(){
    CGEventTapEnable(tap, false);
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CFRelease(runLoopSource);
    CFRelease(tap);
}


void terminalSetup(){
    system("clear");
    system("stty -echo");
}


void terminalTeardown(){
    system("stty echo");
}