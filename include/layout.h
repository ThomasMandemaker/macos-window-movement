#ifndef LAYOUT_H
#define LAYOUT_H

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

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

enum Direction {
    UP = 126,
    RIGHT = 124,
    DOWN = 125,
    LEFT = 123
};

void FreeAll();
Panel* InitLayout(CGSize size, CGPoint point);
void SplitVertical(Panel* panelToSplit);
void SplitHorizontal(Panel* panelToSplit);
Panel* NextPanel(enum Direction dir);

#endif // LAYOUT_H

