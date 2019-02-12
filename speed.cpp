#include "speed.hpp"
#include "iostream"
#define ACCELERATION 100

using namespace std;
Speed::Speed(int maxOffset){
    this->maxOffset = maxOffset;
    negativeOffset = 0.0;
    positiveOffset = 0.0;
    forwardOffset = 0.0;
    backwardOffset = 0.0;
}

DirectionXY Speed::getOffsetXY(DirectionalVector dv){
    DirectionXY p1 = dv.p1;
    DirectionXY p2 = dv.p2;

    double vl = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    currentA = (forwardOffset - backwardOffset) / vl;

    return DirectionXY((p2.x - p1.x)*currentA, (p2.y - p1.y)*currentA);
}

double Speed::getCurrentA() { return abs(currentA); }

void Speed::accelarate(){
    if (forwardOffset <= 0.0){
        forwardOffset = 1;
    }

    if (forwardOffset < maxOffset){
        forwardOffset += forwardOffset/ACCELERATION + 0.1;
    }
}

void Speed::slowdown(){
    if (forwardOffset > 0){
        forwardOffset -= forwardOffset/ACCELERATION;
    }
    if (forwardOffset <= 0) { forwardOffset = 0.0; }
}

void Speed::backward_accelarate(){

    if (backwardOffset == 0.0){
        backwardOffset = 1;
    }
    if (backwardOffset < maxOffset){
        backwardOffset += backwardOffset/ACCELERATION + 0.1;
    }
}

void Speed::backward_slowdown(){
    if (backwardOffset > 0){
        backwardOffset -= backwardOffset/ACCELERATION;
    }
    if (backwardOffset <= 0) { backwardOffset = 0.0; }
}

