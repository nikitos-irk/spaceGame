#include "nozzle.hpp"

#include "ship.hpp"

namespace space {
Nozzle::Nozzle(Ship const& ship, primitive::Point coordinate)
    : ship_{ship},
      border_{coordinate,
              {coordinate.x - double(size.width)/2,
               coordinate.y + double(size.height)},
              {coordinate.x + double(size.width)/2,
               coordinate.y + double(size.height)}},
      border_origin_{border_} {}

double Nozzle::getTiltAngel() const
{
    return ship_.getTiltAngel();
}

void Nozzle::update()
{
    // double offsetLength = ship_.get_speed().get_current_a();
    double offsetLength = ship_.get_speed().get_current_a_signed();
    colors.update_(ship_.get_speed().get_current_a_signed());
    border_.clear();
    std::copy(border_origin_.begin(), border_origin_.end(),
        std::back_inserter(border_));
    double Cx, Cy, Cx2, Cy2;
    int multiply = -1;
    
    if (offsetLength < 0) {
        multiply = 1;
    }
    offsetLength = std::abs(offsetLength);
    
    Cx = (border_[1].x - border_[0].x) * offsetLength;
    Cy = (border_[1].y - border_[0].y) * multiply * offsetLength;

    Cx2 = (border_[0].x - border_[2].x) * offsetLength;
    Cy2 = (border_[0].y - border_[2].y) * multiply * offsetLength;
    border_[1].x += Cx;
    border_[1].y += Cy;

    border_[2].x -= Cx2;
    border_[2].y -= Cy2;
}

void Nozzle::rotate(bool clockwise)
{
    int sign = clockwise ? 1 : -1;
    ship_.rotatePointsInVector(border_origin_,
        ship_.get_initial_median_intersaction(), sign * kAngle);
}
}  // namespace space
