#include "keyboard.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <stdint.h>
#include <stdio.h>

EventCallback eventCallback = NULL;

void init_listener(EventCallback callBack) {
    if (!callBack) {
        printf("No callback given, exiting...");
        return;
    }

    eventCallback = callBack;

    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef r = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL);
    if (!r) {
        printf("Failed");
        return;
    }


    CFRunLoopSourceRef loop = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, r, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(),loop, kCFRunLoopCommonModes);
    CGEventTapEnable(r, true);
    CFRunLoopRun();
}

CGEventRef CGEventCallback(CGEventTapProxy  proxy,
  CGEventType type, CGEventRef event, void * __nullable userInfo) {
    CGEventFlags flags = CGEventGetFlags(event);
    if (!!(flags & kCGEventFlagMaskAlternate)) {
        return event;
    }

    int64_t key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    eventCallback(key);
    return event;
}
