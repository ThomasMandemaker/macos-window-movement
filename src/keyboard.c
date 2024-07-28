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
    CFRunLoopRun();
}

CGEventRef CGEventCallback(CGEventTapProxy  proxy,
  CGEventType type, CGEventRef event, void * __nullable callback) {
    CGEventFlags flags = CGEventGetFlags(event);
    if (!(flags & kCGEventFlagMaskAlternate) || !callback) {
        return event;
    }

    int64_t key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    ((EventCallback)callback)(key);
    return event;
}
