#ifndef WINDOW_H
#define WINDOW_H
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include "layout.h"


void ResizeFocusedWindow(CGSize size, CGPoint point);
//void ResizeFocusedWindow(int height, int width, int x, int y);
void release();

#endif // WINDOW_H
