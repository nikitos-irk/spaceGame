#include "ship.hpp"

namespace space {

Ship::Ship(primitive::Point coordinate)
    : border_{coordinate,
              {coordinate.x - double(size.width)/2,
               coordinate.y + double(size.height)},
              {coordinate.x + double(size.width)/2,
               coordinate.y + double(size.height)}},
      left_nozzle_{*this, {coordinate.x - double(size.width)/2,
                    coordinate.y + double(size.height)}},
      right_nozzle_{*this, {coordinate.x + double(size.width)/2,
                    coordinate.y + double(size.height)}} {}

double Ship::getTiltAngel() const
{
    const auto& center = border_.front();
    primitive::Point tmp{center.x, center.y - 10};
    primitive::Point middle{(*(border_.begin()+1)).x/2 + (*(border_.begin()+2)).x/2,
                           (*(border_.begin()+1)).y/2 + (*(border_.begin()+2)).y/2};

    double x1 = center.x;
    double y1 = center.y;
    double x2 = center.x;
    double y2 = center.y - 10;
    double x3 = center.x;
    double y3 = center.y;
    double x4 = middle.x;
    double y4 = middle.y;

    return atan((y2 - y1)/(x2 - x1)) - atan((y4 - y3)/(x4 - x3));
}

void Ship::update()
{
    auto mediana = primitive::median(border_[1], border_[2]);
    speed_.getOffsetXY({mediana.x, mediana.y}, {border_[0].x, border_[0].y});
    left_nozzle_.update();
    right_nozzle_.update();
}

void Ship::slowdown(){
    speed_.slowdown();
}
void Ship::accelarate(){
    speed_.accelarate();
}
void Ship::backwardSlowdown(){
    speed_.backwardSlowdown();
}
void Ship::backwardAccelarate(){
    speed_.backwardAccelarate();
}

}  // namespace space
