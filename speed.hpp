#ifndef SPEED_H
#define SPEED_H

#include "SpaceShip.hpp"

class Speed{

private:
    double maxOffset;

    double forwardOffset;
    double backwardOffset;

    double negativeOffset;
    double positiveOffset;

    double backward_currentOffset;
    double backward_negativeOffset;
    double backward_positiveOffset;
public:
    Speed(int);
    void slowdown();
    void accelarate();
    void backward_slowdown();
    void backward_accelarate();
    DirectionXY getOffsetXY(DirectionalVector);
};

#endif // SPEED_H
