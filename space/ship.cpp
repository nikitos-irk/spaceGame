#include "ship.hpp"

#include "primitive/line.hpp"
#include "iostream"

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
                    coordinate.y + double(size.height)}},
      mass {1000}
{
    initial_median_intersection_ = CalcMedianIntersaction();
    rotation_delay_ = primitive::delay(kRotationDelay);
    gravity_ = new Gravity(mass);
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
    left_nozzle_.update();
    right_nozzle_.update();
}

primitive::Direction Ship::getOffset(std::list<SpaceObject*>* stars, bool gravity)
{
    auto tmp = get_initial_median_intersaction();
    auto speed_offset = speed_.getOffsetXY({tmp.x, tmp.y}, {border_[0].x, border_[0].y});
    
    if (gravity) {
        auto directionXY_gravity = gravity_->get_offset(stars, get_initial_median_intersaction());

        speed_offset.x -= directionXY_gravity.x;
        speed_offset.y -= directionXY_gravity.y;

        // std::cout << "speed_offset.x =          " << speed_offset.x << std::endl;
        // std::cout << "directionXY_gravity.x =   " << directionXY_gravity.x << std::endl;
        
        // if (speed_offset.x * directionXY_gravity.x > 0 ){
        //     if (abs(speed_offset.x) - abs(directionXY_gravity.x) < 0.001 ) {
        //         directionXY_gravity.x = 0;
        //     }
        // }
        // if (speed_offset.y * directionXY_gravity.y > 0 ){
        //     if (abs(speed_offset.y) - abs(directionXY_gravity.y) < 0.001 ) {
        //         directionXY_gravity.y = 0;
        //     }
        // }
        // if (speed_offset.y - directionXY_gravity.y < 0.000001 ){
        //     directionXY_gravity.y = 0;
        // }
            
        gravity_->update_inertia(speed_offset);
    }
    
    return speed_offset;
}

void Ship::slowdown()
{
    speed_.slowdown();
}

void Ship::accelarate()
{
    speed_.accelarate();
}

void Ship::backwardSlowdown()
{
    speed_.backwardSlowdown();
}

void Ship::backwardAccelarate()
{
    speed_.backwardAccelarate();
}

ProjectilePtr Ship::shoot()
{
    if (shoot_delay_ > primitive::now()) { return nullptr; }
    shoot_delay_ = primitive::delay(kShootingDelay);

    auto mediana = primitive::median(border_[1], border_[2]);
    double diff_x = (mediana.x - border_[0].x)/5;
    double diff_y = (mediana.y - border_[0].y)/5;

    return std::make_unique<Projectile>(primitive::Point{border_[0].x - diff_x,
                                                         border_[0].y - diff_y},
                                        primitive::Direction{-diff_x, -diff_y});
}

}  // namespace space
