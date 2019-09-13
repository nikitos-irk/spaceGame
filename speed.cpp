#include "speed.hpp"

#include <cmath>

constexpr auto kAcceleration = 100;

Speed::Speed(int max_offset){
    this->max_offset_ = max_offset;
    negative_offset_ = 0.0;
    positive_offset_ = 0.0;
    forward_offset_ = 0.0;
    backward_offset_ = 0.0;
}

DirectionXY Speed::getOffsetXY(DirectionalVector dv){
    DirectionXY p1 = dv.p1;
    DirectionXY p2 = dv.p2;

    double vl = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    current_a_ = (forward_offset_ - backward_offset_) / vl;

    return DirectionXY((p2.x - p1.x)*current_a_, (p2.y - p1.y)*current_a_);
}

double Speed::get_current_a() const { return abs(current_a_); }

void Speed::accelarate(){
    if (forward_offset_ <= 0.0){
        forward_offset_ = 1;
    }

    if (forward_offset_ < max_offset_){
        forward_offset_ += forward_offset_/kAcceleration + 0.1;
    }
}

void Speed::slowdown(){
    if (forward_offset_ > 0){
        forward_offset_ -= forward_offset_/kAcceleration;
    }
    if (forward_offset_ <= 0) { forward_offset_ = 0.0; }
}

void Speed::backwardAccelarate(){

    if (backward_offset_ == 0.0){
        backward_offset_ = 1;
    }
    if (backward_offset_ < max_offset_){
        backward_offset_ += backward_offset_/kAcceleration + 0.1;
    }
}

void Speed::backwardSlowdown(){
    if (backward_offset_ > 0){
        backward_offset_ -= backward_offset_/kAcceleration;
    }
    if (backward_offset_ <= 0) { backward_offset_ = 0.0; }
}
