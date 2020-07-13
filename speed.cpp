#include "speed.hpp"

#include <cmath>
#include <cstdlib>

namespace primitive {

constexpr auto kAcceleration = 100;

Speed::Speed(double max_offset) : max_offset_{max_offset} {}

Direction Speed::getOffsetXY(Direction p1, Direction p2)
{
    double vl = std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
    current_a_ = (forward_offset_ - backward_offset_) / vl;

    return {(p2.x - p1.x)*current_a_, (p2.y - p1.y)*current_a_};
}

double Speed::get_current_a() const { return std::abs(current_a_); }
double Speed::get_current_a_signed() const { return current_a_; }

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

// MOVEMENT

Movement::Movement(){
    auto LO{-3.0}, HI{3.0};
    shiftX_ = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    shiftY_ = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
};
Direction Movement::getOffset() { return Direction{shiftX_, shiftY_}; }

}
