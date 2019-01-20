#include "speed.hpp"


Speed::Speed(int maxOffset){
    this->maxOffset = maxOffset;
    negativeOffset = 0;
    positiveOffset = 0;
    currentOffset = 0;
}

DirectionXY Speed::getOffsetXY(DirectionalVector dv){
    DirectionXY p1 = dv.p1;
    DirectionXY p2 = dv.p2;

    currentOffset = positiveOffset - negativeOffset;

    double vl = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
//    cout << "VL = " << vl << endl;
//    cout << "[" << p1.x << ";" << p1.y << "]" << endl;
//    cout << "[" << p2.x << ";" << p2.y << "]" << endl;
    double k = currentOffset / vl;
    double x = p1.x + (p2.x - p1.x) * k;
    double y = p1.x + (p2.x - p1.x) * k;
    cout << "[" << p2.x - x << ";" << p2.y - y << "]" << endl;
    return DirectionXY(p2.x - x, p2.y - y);
}

void Speed::slowdown(){
    if (fabs(maxOffset - negativeOffset) < 0.005f) { return; }
    negativeOffset += maxOffset/10;
}

void Speed::accelarate(){
    if (fabs(maxOffset - positiveOffset) < 0.005f) { return; }
    positiveOffset += maxOffset/100;
}
