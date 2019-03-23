#include "common.hpp"

Point get_rotated_point(Point center_point, Point rotated_point, double angle){
    if (0 == angle){
        angle = (rand() % 360)/M_PI;
    }
    point P(center_point.x, center_point.y);
    point Q(rotated_point.x, rotated_point.y);
    point P_rotated = (P-Q) * polar(1.0, angle) + Q;
    return Point(P_rotated.real(), P_rotated.imag());
}

DirectionXY::DirectionXY(double x, double y){
    this->x = x;
    this->y = y;
}

DirectionXY::DirectionXY(){
    this->x = 0;
    this->y = 0;
}

Common::Common() {

}
