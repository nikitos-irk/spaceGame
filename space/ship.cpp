#include "ship.hpp"

#include "primitive/line.hpp"

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
                    coordinate.y + double(size.height)}}
{
    initial_median_intersection_ = CalcMedianIntersaction();
    rotation_delay_ = primitive::delay(kRotationDelay);
}

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

void Ship::rotate(bool clockwise)
{
  if (rotation_delay_ > primitive::now()) { return; }

  int sign = clockwise ? 1 : -1;
  rotatePointsInVector(border_, get_initial_median_intersaction(), sign * kAngle);
  left_nozzle_.rotate(clockwise);
  right_nozzle_.rotate(clockwise);

  rotation_delay_ = primitive::delay(kRotationDelay);
}

primitive::Point Ship::CalcMedianIntersaction() const
{
    double x1 = border_[0].x;
    double y1 = border_[0].y;
    double x2 = (border_[2].x + border_[1].x)/2;
    double y2 = (border_[2].y + border_[1].y)/2;

    double x3 = border_[1].x;
    double y3 = border_[1].y;
    double x4 = (border_[0].x + border_[2].x)/2;
    double y4 = (border_[0].y + border_[2].y)/2;

    return primitive::Line{{x1, y1}, {x2, y2}}.intersect({{x3, y3}, {x4, y4}});
}

void Ship::rotatePointsInVector(std::vector<primitive::Point>& points,
                                primitive::Point initial_median_intersection,
                                double angle) const
{
    for (auto iter = points.begin(); iter != points.end(); ++iter) {
        iter->rotate(initial_median_intersection, angle);
    }
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
