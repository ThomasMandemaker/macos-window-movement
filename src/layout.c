#include "layout.h"
#include "direction.h"
#include <CoreFoundation/CoreFoundation.h>
#include <assert.h>
#include <math.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int GetCompAxis(Panel* panel, Direction dir) {
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

Panel* CreatePanel(Layout* layout, CGSize size, CGPoint point) {
    assert(layout);
    Panel* panel = (Panel*)malloc(sizeof(Panel));
    panel->size = size;
    panel->point = point;
    panel->up = NULL;
    panel->right = NULL;
    panel->down = NULL;
    panel->left = NULL;
    panel->parent = layout;
    layout->panelList[layout->panelSize++] = panel;
    return panel;
}


Panel* PanelInDirection(Panel* panel, Direction dir) {
    switch (dir) {
        case UP: return panel->up;
        case RIGHT: return panel->right;
        case DOWN: return panel->down;
        case LEFT: return panel->left;
        default: return NULL;
    }
}

void SetPanelInDirection(Panel* target, Panel* value, Direction dir) {
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

Layout* InitLayout(CGSize size, CGPoint point) {
    Layout* layout = (Layout*)malloc(sizeof(Layout));
    layout->panelList = (Panel**)malloc(sizeof(Panel*)*20);
    layout->panelSize = 0;
    layout->currentPanel = CreatePanel(layout, size, point);

    return layout;
}


void DestroyLayout(Layout* layout) {
    if (!layout || !layout->panelList) {
        return;
    }


    for (int i = 0; i < layout->panelSize; i++) {
        free(layout->panelList[i]);
    }

    free(layout);
}

Panel* FindClosestPanel(int compAxis, Panel* nextPanel, Panel* closestPanel, Direction dir) {
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

void SwapPanels(Panel* panel, Panel* p, Direction dir) {
    Panel* pDir = PanelInDirection(p, dir);

    if (!pDir || pDir != panel) {
        return;
    }

    int compAxis = GetCompAxis(p, dir);

    Panel* pDirection = FindClosestPanel(compAxis, panel, panel, PerpendicularDirection(dir));
    if (!pDirection || pDirection == panel) {
        pDirection = FindClosestPanel(compAxis, panel, panel, SwapDirection(PerpendicularDirection(dir)));
    }

    if ((!pDirection || pDirection == panel) && PanelInDirection(panel, dir)) {
        pDirection = PanelInDirection(panel, dir);
    }

    if (!pDirection || pDirection == panel) {
        pDirection = NULL;
    }

    SetPanelInDirection(p, pDirection, dir);
}

void RemapCloserPanel(Panel* panel, Panel* panelToSplit, Direction dir, Direction dir2) {
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

void SplitVertical(Layout* layout, Panel* panelToSplit) {
    assert(layout && panelToSplit);

    if (panelToSplit->parent != layout) {
        return;
    }

    Panel* newPanel = CreatePanel(layout,
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

void SplitHorizontal(Layout* layout, Panel* panelToSplit) {
    assert(layout && panelToSplit);

    if (panelToSplit->parent != layout) {
        return;
    }
    Panel* newPanel = CreatePanel(layout,
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

void RemovePanel(Layout* layout, Panel* panelToDelete) {
    assert(layout && panelToDelete);

    if (panelToDelete->parent != layout || layout->panelSize <= 0) {
        return;
    }

    int index = -1;
    for (int i = 0; i < layout->panelSize; i++) {
        Panel* panel = layout->panelList[i];
        if (panel == panelToDelete) {
            index = i;
            continue;
        }

        SwapPanels(panelToDelete, panel, UP);
        SwapPanels(panelToDelete, panel, RIGHT);
        SwapPanels(panelToDelete, panel, DOWN);
        SwapPanels(panelToDelete, panel, LEFT);
    }

    assert(index != -1);

    free(panelToDelete);

    layout->panelSize--;
    if (index == layout->panelSize) {
        return;
    }

    Panel* panel = layout->panelList[layout->panelSize];
    layout->panelList[index] = panel;

}

Panel* NextPanel(Layout* layout, Direction dir) {
    Panel* nextPanel = PanelInDirection(layout->currentPanel, dir);
    
    if (nextPanel == NULL) {
        return NULL;
    }

    layout->currentPanel = nextPanel;
    return nextPanel;
}

void AdjustSize(Layout* layout, Panel* panel, CGSize size) {
    assert(layout && panel);
    assert(panel->parent == layout);

    int dWidth = size.width - panel->size.width;
    int dHeight = size.height - panel->size.height;

    if (dWidth == 0 && dHeight == 0) {
        return;
    }

    int count = 0;
    Panel* p = panel->up;    

    Panel** pList = (Panel**)malloc(sizeof(Panel*)*20);
    int pCount = 0;

    // Up
    while (p && count < 100) {
        p = p->right;
        count++;
        // If the end x of the panel is less than the point x, ignore it
        if (p->point.x + p->size.width < panel->point.x) {
            continue; // Can't actually continue REMOVE
        }

        // If the point x of the panel is more than the end x, ignore it
        if (p->point.x > panel->point.x + panel->size.width) {
            continue; // Can't actually continue REMOVE
        }

        // If the point is within, check if the y is against the current panel
        if (floor(p->point.y + p->size.height) != floor(panel->point.y)) {
            continue; // Can't actually continue REMOVE
        }

        pList[pCount++] = p;
    }
    
    free(pList);
    


}
