#ifndef SPEED_H
#define SPEED_H

#include "SpaceShip.hpp"

class Speed{

private:
    double maxOffset;
    double currentOffset;
    double negativeOffset;
    double positiveOffset;
public:
    Speed(int);
    void slowdown();
    void accelarate();
    DirectionXY getOffsetXY(DirectionalVector);
};

#endif // SPEED_H
