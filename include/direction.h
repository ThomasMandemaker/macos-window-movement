#ifndef DIRECTION_H
#define DIRECTION_H

enum Direction {
    UP = 126,
    RIGHT = 124,
    DOWN = 125,
    LEFT = 123
};

enum Direction SwapDirection(enum Direction dir);
enum Direction PerpendicularDirection(enum Direction dir);

#endif // DIRECTION_H
