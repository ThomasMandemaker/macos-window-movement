#include "layout.h"
#include "direction.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Panel* currentPanel = NULL;
Panel** panelList = NULL;
int panelSize = 0;

void Print(char* text) {
    FILE* a = fopen("/Users/thomasmandemaker/Desktop/Projects/tile-manager/im-having-a-fucking-keystroke.log", "a");
    fprintf(a, text, "");
    fflush(a);
}

void PrintPanelProperties(Panel* panel) {
    FILE* a = fopen("/Users/thomasmandemaker/Desktop/Projects/tile-manager/im-having-a-fucking-keystroke.log", "a");
    fprintf(a, "\n\nIndex is: %d\n", panel->index);
    fprintf(a, "\nHeight is: %f\n", panel->size.height);
    fprintf(a, "\nWidth is: %f\n", panel->size.width);
    fprintf(a, "\nX is: %f\n", panel->point.x);
    fprintf(a, "\nY is: %f\n", panel->point.y);
    fflush(a);
}

int GetCompAxis(Panel* panel, enum Direction dir) {
    assert(panel != NULL);

    switch (dir) {
        case UP:
        case DOWN:
            return panel->point.x;
        case RIGHT:
        case LEFT:
            return panel->point.y;
        default:
            return -1;
    }
}

Panel* CreatePanel(CGSize size, CGPoint point) {
    Panel* panel = (Panel*)malloc(sizeof(Panel));
    panel->size = size;
    panel->point = point;
    panel->index = panelSize;
    panel->up = NULL;
    panel->right = NULL;
    panel->down = NULL;
    panel->left = NULL;
    panelList[panelSize] = panel;
    panelSize = panelSize + 1;
    return panel;
}


Panel* PanelInDirection(Panel* panel, enum Direction dir) {
    switch (dir) {
        case UP: return panel->up;
        case RIGHT: return panel->right;
        case DOWN: return panel->down;
        case LEFT: return panel->left;
        default: return NULL;
    }
}

void SetPanelInDirection(Panel* target, Panel* value, enum Direction dir) {
    switch (dir) {
        case UP: target->up = value;
                 break;
        case RIGHT: target->right = value;;
                 break;
        case DOWN: target->down = value;
                 break;
        case LEFT: target->left = value;
                 break;
        default: return;
    }
}

Panel* InitLayout(CGSize size, CGPoint point) {
    if (!panelList) {
        panelList = (Panel**)malloc(sizeof(Panel*)*20);
    }

    if (!currentPanel) {
        currentPanel = CreatePanel(size, point);
    }

    return currentPanel;
}

Panel* FindClosestPanel(int compAxis, Panel* nextPanel, Panel* closestPanel, enum Direction dir) {
        if (!nextPanel) {
            return closestPanel; // Means we're at the bottom
        }

        int nextPanelAxis;
        int closestAxis;
        switch (dir) {
            case DOWN:
            case UP: {
                nextPanelAxis = nextPanel->point.y;
                closestAxis = closestPanel->point.y;
            } break;
            case LEFT:
            case RIGHT: {
                nextPanelAxis = nextPanel->point.x;
                closestAxis = closestPanel->point.x;
            } break;
            default:
                    return NULL;
        }


        if (abs(nextPanelAxis - compAxis) < abs(closestAxis - compAxis))  {
            closestPanel = nextPanel;
        }

        nextPanel = PanelInDirection(nextPanel, dir);

        return FindClosestPanel(compAxis, nextPanel, closestPanel, dir);
}

void SwapPanels(Panel* panel, Panel* p, enum Direction dir) {
    Panel* pDir = PanelInDirection(p, dir);

    if (!pDir || pDir->index != panel->index) {
        return;
    }

    int compAxis = GetCompAxis(p, dir);

    Panel* pDirection = FindClosestPanel(compAxis, panel, panel, PerpendicularDirection(dir));
    if (!pDirection || pDirection->index == panel->index) {
        pDirection = FindClosestPanel(compAxis, panel, panel, SwapDirection(PerpendicularDirection(dir)));
    }

    if ((!pDirection || pDirection->index == panel->index) && PanelInDirection(panel, dir)) {
        pDirection = PanelInDirection(panel, dir);
    }

    if (!pDirection || pDirection->index == panel->index) {
        pDirection = NULL;
    }

    SetPanelInDirection(p, pDirection, dir);
}

void RemapCloserPanel(Panel* panel, Panel* panelToSplit, enum Direction dir, enum Direction dir2) {
    int panelCompAxis =  GetCompAxis(panel, dir);

    Panel* closestPanel = FindClosestPanel(panelCompAxis, PanelInDirection(panelToSplit, dir), PanelInDirection(panelToSplit, dir), dir2);
    SetPanelInDirection(panel, closestPanel, dir);

    if (!closestPanel) {
        return;
    }

    int closestCompAxis = GetCompAxis(closestPanel, dir);
    int dXCurrent = abs(closestCompAxis - GetCompAxis(PanelInDirection(closestPanel, SwapDirection(dir)), dir));
    int dXClosesPanel = abs(closestCompAxis - GetCompAxis(panel, dir));
    
    if (dXCurrent > dXClosesPanel) {
        SetPanelInDirection(closestPanel, panel, SwapDirection(dir));
    }
}

void SplitVertical(Panel* panelToSplit) {
    Panel* newPanel = CreatePanel(
            CGSizeMake(panelToSplit->size.width, panelToSplit->size.height/2),
            CGPointMake(panelToSplit->point.x, panelToSplit->point.y + panelToSplit->size.height/2));

    panelToSplit->size.height = newPanel->size.height;

    // DOWN
    if (panelToSplit->down != NULL) {
        newPanel->down = panelToSplit->down;
        panelToSplit->down->up = newPanel;
    }
    panelToSplit->down = newPanel;

    // UP
    newPanel->up = panelToSplit;

    // LEFT
    RemapCloserPanel(newPanel, panelToSplit, LEFT, DOWN);

    // RIGHT
    RemapCloserPanel(newPanel, panelToSplit, RIGHT, DOWN);
}

void SplitHorizontal(Panel* panelToSplit) {
    Panel* newPanel = CreatePanel(
            CGSizeMake(panelToSplit->size.width/2, panelToSplit->size.height),
            CGPointMake(panelToSplit->point.x + panelToSplit->size.width/2, panelToSplit->point.y));
    panelToSplit->size.width = newPanel->size.width;

    // RIGHT
    if (panelToSplit->right != NULL) {
        newPanel->right = panelToSplit->right;
        panelToSplit->right->left = newPanel;
    }
    panelToSplit->right = newPanel;

    // LEFT
    newPanel->left = panelToSplit;

    // UP
    RemapCloserPanel(newPanel, panelToSplit, UP, RIGHT);

    // DOWN
    RemapCloserPanel(newPanel, panelToSplit, DOWN, RIGHT);
}

void FreeAll() {
    for (int i = 0; i < panelSize; i++) {
        free(panelList[i]);
    }

    free(panelList);
}


void RemovePanel(Panel* panel) {
    if (!panel) {
        return;
    }

    for (int i = 0; i < panelSize; i++) {
        if (i == panel->index) {
            continue;
        }

        Panel* p = panelList[i];
        
        SwapPanels(panel, p, UP);
        SwapPanels(panel, p, RIGHT);
        SwapPanels(panel, p, DOWN);
        SwapPanels(panel, p, LEFT);
    }

    int index = panel->index;

    free(panelList[index]);

    if (index != panelSize) {
        Panel* pan = panelList[panelSize-1];
        pan->index = index;
        panelList[index] = pan;
    }

    panelSize = panelSize-1;
}

Panel* NextPanel(enum Direction dir) {
    Panel* nextPanel = PanelInDirection(currentPanel, dir);
    
    if (nextPanel == NULL) {
        return NULL;
    }

    currentPanel = nextPanel;
    return currentPanel;
}
