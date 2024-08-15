#ifndef DIRECTION_H
#define DIRECTION_H

typedef enum{
    UP = 126,
    RIGHT = 124,
    DOWN = 125,
    LEFT = 123
} Direction ;

Direction SwapDirection(Direction dir);
Direction PerpendicularDirection(Direction dir);

#endif // DIRECTION_H
