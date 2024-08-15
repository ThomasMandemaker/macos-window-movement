#ifndef WINDOW_H
#define WINDOW_H
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include "layout.h"


void ResizeFocusedWindow(CGSize size, CGPoint point);
void release();

#endif // WINDOW_H
