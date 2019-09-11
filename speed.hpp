#ifndef SPEED_H
#define SPEED_H

#include "common.hpp"

class Speed{
private:
    double max_offset_;
    double forward_offset_;
    double backward_offset_;
    double negative_offset_;
    double positive_offset_;
    double current_a_;  // TODO: What is it?

public:
    Speed(int);
    void slowdown();
    void accelarate();
    void backwardSlowdown();
    void backwardAccelarate();
    DirectionXY getOffsetXY(DirectionalVector);
    double get_current_a();
};

#endif // SPEED_H
