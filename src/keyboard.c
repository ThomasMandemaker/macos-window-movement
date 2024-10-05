#include "keyboard.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <stdint.h>
#include <stdio.h>

void init_listener(EventCallback callBack) {
    if (!callBack) {
        printf("No callback given, exiting...");
        return;
    }

    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, callBack);
    if (!eventTap) {
        printf("Failed to create event tap");
        return;
    }


    CFRunLoopSourceRef loop = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), loop, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
//    CFRunLoopRun();
}

CGEventRef CGEventCallback(CGEventTapProxy  proxy,
  CGEventType type, CGEventRef event, void * __nullable callback) {

    CGEventFlags flags = CGEventGetFlags(event);
    if (!(flags & kCGEventFlagMaskAlternate) || !callback) {
        return event;
    }

    flags = flags & ~kCGEventFlagMaskAlternate;
    CGEventSetFlags(event, flags);

    int64_t key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    switch(key) {
        case 0: // A
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 18);
            break;
        case 1: // S
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 19);
            break;
        case 2: // D
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 20);
            break;
        case 3: // F
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 21);
            break;
        case 5: // G
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 23);
            break;
        case 4: // H
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 22);
            break;
        case 38: // J
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 26);
            break;
        case 40: // K
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 28);
            break;
        case 37: // L
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 25);
            break;
        case 41: // ;
            CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 29);
            break;
        default:
            break;
    }

    ((EventCallback)callback)(key);

    return event;
}
