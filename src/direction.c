#include "direction.h"
#include <stddef.h>

Direction SwapDirection(Direction dir) {
    switch (dir) {
        case UP: return DOWN;
        case RIGHT: return LEFT;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        default: return NULL;
    }
}

Direction PerpendicularDirection(Direction dir) {
    switch (dir) {
        case UP: return RIGHT;
        case RIGHT: return DOWN;
        case DOWN: return RIGHT;
        case LEFT: return LEFT;
        default: return NULL;
    }
}
