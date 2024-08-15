#ifndef LAYOUT_H
#define LAYOUT_H

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include "direction.h"

typedef struct Panel Panel;
typedef struct Layout Layout;

struct Panel {
    CGSize size;
    CGPoint point;
    Layout* parent;
    Panel* up;
    Panel* right;
    Panel* down;
    Panel* left;
};

struct Layout{
    Panel** panelList;
    int panelSize;
    Panel* currentPanel;
};

Layout* InitLayout(CGSize size, CGPoint point);
void DestroyLayout(Layout* layout);
void SplitVertical(Layout* layout, Panel* panelToSplit);
void SplitHorizontal(Layout* layout, Panel* panelToSplit);
Panel* NextPanel(Layout* layout, Direction dir);
void RemovePanel(Layout* layout, Panel* panel);
void PrintPanelProperties(char* prefix, Panel* panel);

#endif // LAYOUT_H

