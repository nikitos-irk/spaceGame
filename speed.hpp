#ifndef SPEED_H
#define SPEED_H

#include <cmath>

struct DirectionXY{
    double x;
    double y;
    DirectionXY(double, double);
    DirectionXY();

    DirectionXY& operator *=(int value){
        x *= value;
        y *= value;
        return *this;
    }
    DirectionXY(const DirectionXY& tmpXY) : x(tmpXY.x), y(tmpXY.y){}
};

struct DirectionalVector{
    DirectionXY p1, p2;
    DirectionalVector();
    DirectionalVector(DirectionXY, DirectionXY);
};

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
