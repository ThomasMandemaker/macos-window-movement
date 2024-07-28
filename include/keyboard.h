#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Carbon/Carbon.h>
#include <stdint.h>

typedef void (*EventCallback)(int64_t);

CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
void init_listener(EventCallback callBack);

#endif // KEYBOARD_H 

