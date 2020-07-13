#ifndef SPEED_H
#define SPEED_H

#include "primitive/point.hpp"
namespace primitive {

class Speed {

private:
    double max_offset_{};
    double forward_offset_{};
    double backward_offset_{};
    double negative_offset_{};
    double positive_offset_{};
    double current_a_{};  // TODO: What is it?

public:
    explicit Speed(double);
    void slowdown();
    void accelarate();
    void backwardSlowdown();
    void backwardAccelarate();
    Direction getOffsetXY(Direction, Direction);
    double get_current_a() const;
    double get_current_a_signed() const;
};

class Movement {

private:
    // BASE SPEED
    float shiftX_;
    float shiftY_;

public:
    Movement();
    Direction getOffset();

};

}

#endif // SPEED_H
