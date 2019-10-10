#include "explosion.hpp"

#include "primitive/point.hpp"
#include "space/asteroid.hpp"

namespace space {

Fragment::Fragment(ColorGenerator colors, primitive::Point p1,
                                          primitive::Point p2,
                                          primitive::Point p3)
    : colors{colors}
{
    double tmp_x = p1.x/3 + p2.x/3 + p3.x/3;
    double tmp_y = p1.y/3 + p2.y/3 + p3.y/3;

    this->x_shift_ = abs(tmp_x - p1.x)/3 + abs(tmp_x - p2.x)/3 + abs(tmp_x - p3.x)/3;
    this->y_shift_ = abs(tmp_y - p1.y)/3 + abs(tmp_y - p2.y)/3 + abs(tmp_y - p3.y)/3;
    this->angle_ = 0.1 + rand() % 15;

    dots.push_back(p1);
    dots.push_back(p2);
    dots.push_back(p3);
    fragment_shift_delay_ = primitive::delay(kFragmentShiftDelay);
}

void Fragment::update()
{
    if (fragment_shift_delay_ > primitive::now()) { return; }

    double center_x = 0.0;
    double center_y = 0.0;

    int dots_number = dots.size();
    for(auto iter = dots.begin(); iter != dots.end(); ++iter){
        center_x += iter->x / dots_number;
        center_y += iter->y / dots_number;
        iter->x += this->x_shift_;
        iter->y += this->y_shift_;
    }

    for(auto iter = dots.begin(); iter != dots.end(); ++iter){
        iter->rotate({center_x, center_y}, angle_);
    }

    fragment_shift_delay_ = primitive::delay(kFragmentShiftDelay);
}

Explosion::Explosion(primitive::Point p, space::Asteroid const& ast)
    : p_{p}
{
    destroy_time_ = primitive::delay(kExplosionLifeTime);
    auto p_center = ast.center();

    int random_number_of_ragments = 5 + rand() % 10;
    double angle_diff = 360 / random_number_of_ragments;
    auto p_first = ast.get_points().begin();
    auto p_second = p_first + 1;

    while (p_second != ast.get_points().end()){
        fragments_.push_back(Fragment(ast.colors, *p_first, *p_second, p_center));
        ++p_first;
        ++p_second;
    }
    p_second = ast.get_points().begin();
    fragments_.push_back(Fragment(ast.colors, *p_first, *p_second, p_center));
}

void Explosion::move(primitive::Direction d)
{
    for (auto& part: fragments_) {
        for(auto& p: part.dots) {
            p.move(d);
        }
    }
}

void Explosion::update()
{
    for (auto& part: fragments_) part.update();
}

}  // namespace space

