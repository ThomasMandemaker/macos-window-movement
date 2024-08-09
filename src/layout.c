#include "layout.h"
#include <stdio.h>
#include <stdlib.h>

Panel* currentPanel;
Panel** panelList;
int panelSize = 0;

void Print(char* text) {
    FILE* a = fopen("/Users/thomasmandemaker/Desktop/Projects/tile-manager/im-having-a-fucking-keystroke.log", "a");
    fprintf(a, text, "");
    fflush(a);
}



// Print the relative connections of a panel 24x24
void PrintMap(Panel* panel) {
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

enum Direction SwapDirection(enum Direction dir) {
    switch (dir) {
        case UP: return DOWN;
        case RIGHT: return LEFT;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        default: return NULL;
    }
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

Panel* InitLayout(CGSize size, CGPoint point) {
    panelList = (Panel**)malloc(sizeof(Panel*)*20);
    currentPanel = CreatePanel(size, point);

    return currentPanel;
}

Panel* TestRec(int compAxis, Panel* nextPanel, Panel* closestPanel, enum Direction dir) {
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

        return TestRec(compAxis, nextPanel, closestPanel, dir);
}

void SwapPanel(Panel* newPanel, Panel* panelToSplit, enum Direction dir) {
    
    Panel* right = TestRec(newPanel->point.y, panelToSplit->right, panelToSplit->right, DOWN);
    newPanel->right = right;
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
    Panel* left = TestRec(newPanel->point.y, panelToSplit->left, panelToSplit->left, DOWN);
    newPanel->left = left;

    if (left) {
        int dX = left->point.y - left->right->point.y;

        if (abs(dX) > abs((int)(left->point.y - newPanel->point.y))) {
            left->right = newPanel;
        }
    }

    // RIGHT
    Panel* right = TestRec(newPanel->point.y, panelToSplit->right, panelToSplit->right, DOWN);
    newPanel->right = right;

    if (right) {
        int dX = right->point.y - right->left->point.y;

        if (abs(dX) > abs((int)(right->point.y - newPanel->point.y))) {
            right->left = newPanel;
        }
    }
}

// After first time, the thing gets linked
void SplitHorizontal(Panel* panelToSplit) {
    FILE* a = fopen("/Users/thomasmandemaker/Desktop/Projects/tile-manager/im-having-a-fucking-keystroke.log", "a");
    fprintf(a, "\n PanelToSplitIndex ajskdfjakdsfj: %d", panelToSplit->index);
    Panel* newPanel = CreatePanel(
            CGSizeMake(panelToSplit->size.width/2, panelToSplit->size.height),
            CGPointMake(panelToSplit->point.x + panelToSplit->size.width/2, panelToSplit->point.y));
    panelToSplit->size.width = newPanel->size.width;

    // Remap windows
    // RIGHT
    if (panelToSplit->right != NULL) {
        newPanel->right = panelToSplit->right;
        panelToSplit->right->left = newPanel;
    }
    panelToSplit->right = newPanel;

    // LEFT
    newPanel->left = panelToSplit;


    // UP (Check for closest y point)
    Panel* up = TestRec(newPanel->point.x, panelToSplit->up, panelToSplit->up, RIGHT);
    newPanel->up = up;

    if (up) {
        int dX = up->point.x - up->down->point.x;

        if (abs(dX) > abs((int)(up->point.x - newPanel->point.x))) {
            up->down = newPanel;
        }
    }

    // DOWN
    Panel* down = TestRec(newPanel->point.x, panelToSplit->down, panelToSplit->down, RIGHT);
    newPanel->down = down;

    if (down) {
        int dX = down->point.x - down->up->point.x; // 0

        if (abs(dX) > abs((int)(down->point.x - newPanel->point.x))) {
            down->up = newPanel;
        }
    }
}

void FreeAll() {
    for (int i = 0; i < panelSize; i++) {
        free(panelList[i]);
    }

    free(panelList);
}



Panel* NextPanel(enum Direction dir) {
    Panel* nextPanel;

    switch (dir) {
        case UP: nextPanel = currentPanel->up;
                 break;
        case RIGHT: nextPanel = currentPanel->right;
                 break;
        case DOWN: nextPanel = currentPanel->down;
                 break;
        case LEFT: nextPanel = currentPanel->left;
                 break;
        default:
                return NULL;
    }
   
    if (nextPanel == NULL) {
        return NULL;
    }

    currentPanel = nextPanel;
    return currentPanel;
}
