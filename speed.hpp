#ifndef SPEED_H
#define SPEED_H

#include <cmath>
#include "common.hpp"

class Speed{

private:
    double maxOffset;
    double forwardOffset;
    double backwardOffset;
    double negativeOffset;
    double positiveOffset;
    double currentA;
public:
    Speed(int);
    void slowdown();
    void accelarate();
    void backward_slowdown();
    void backward_accelarate();
    DirectionXY getOffsetXY(DirectionalVector);
    double getCurrentA();
};

#endif // SPEED_H
