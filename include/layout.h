#ifndef LAYOUT_H
#define LAYOUT_H

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include "direction.h"

typedef struct Panel Panel;

struct Panel {
    CGSize size;
    CGPoint point;
    CGPoint center;
    int index;
    Panel* up;
    Panel* right;
    Panel* down;
    Panel* left;
};

void FreeAll();
void RemovePanel(Panel* panel);
Panel* InitLayout(CGSize size, CGPoint point);
void SplitVertical(Panel* panelToSplit);
void SplitHorizontal(Panel* panelToSplit);
Panel* NextPanel(enum Direction dir);

#endif // LAYOUT_H

