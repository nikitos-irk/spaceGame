#include "space_object.hpp"

#include <cstdlib>
#include <iostream>

#include "colorschema.hpp"
#include "skeleton.hpp"

#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/line.hpp"

constexpr auto kDisplayDelay = 10ms;

SpaceObject::SpaceObject(SDL_Renderer *renderer,
                         primitive::Point coordinate)
    : renderer_{renderer},
      coordinate_{coordinate}
{
  display_delay_ = primitive::delay(kDisplayDelay);
}

SpaceObject::~SpaceObject(){
    // std::cout << "SpaceObject destructor" << std::endl;
}

bool SpaceObject::isAlive(){ return alive_; }
void SpaceObject::markAsDead() { alive_ = false; }

Asteroid::Asteroid(SDL_Renderer *renderer, primitive::Point coordinate)
    : SpaceObject(renderer, coordinate),
      cg_{{160, 177, 188}, {119, 98, 84}, {128, 0, 0}, {81, 81, 81},
          {76, 62, 54}, {139, 69, 19}}
{
    //TODO: think about direction_x;direction_y

    int error_x = rand() % 10;
    int error_y = rand() % 10;
    int size = rand() % 20;

    pp_.push_back(new primitive::Point{coordinate_.x + error_x, coordinate_.y + error_y});
    pp_.push_back(new primitive::Point{coordinate_.x - size + rand() % 5, coordinate_.y + size + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + rand() % 5, coordinate_.y + size * 2 + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + size + rand() % 5, coordinate_.y + size * 2 + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + size * 2 + rand() % 5, coordinate_.y + size + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + size + rand() % 5, coordinate_.y + rand() % 5});
    
    gravity_ = new space::Gravity(getMass());
    movement = new primitive::Movement();
}

std::vector<primitive::Point*>& Asteroid::get_points(){ return pp_; }

double Asteroid::getTriangleArea(primitive::Point *a, primitive::Point *b) {

    primitive::Point c = getCenterPoint();
    auto ab = sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
    auto bc = sqrt(pow(b->x - c.x, 2) + pow(b->y - c.y, 2));
    auto ca = sqrt(pow(a->x - c.x, 2) + pow(a->y - c.y, 2));
    auto perimeter = ab + bc + ca;

    return sqrt(
        perimeter * (perimeter - ab) * (perimeter - bc) * (perimeter - ca)
    );
}

double Asteroid::getMass() {
    if (massIsReady) { return mass; }
    mass = getArea() * space::asteroidDensity;
    massIsReady = true;
    return mass;
}

double Asteroid::getArea() {
    if (pp_.empty()) { return 0; }
    double area;
    for (auto iter = pp_.begin(); iter != pp_.end() - 1; ++iter) {
        area += getTriangleArea(*iter, *(iter+1));
    }
    return area;
}

Asteroid::~Asteroid(){
    // std::cout << "Asteroid destructor" << std::endl;
}

void out(double x, double y){
    std::cout << x << ":" << y << std::endl;
}

primitive::Point Asteroid::getCenterPoint(){
    double x = 0.0, y = 0.0;
    int size = pp_.size();
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        x += (*iter)->x / size;
        y += (*iter)->y / size;
    }
    return primitive::Point{x, y};
}

void Asteroid::fill(){
    
    primitive::Point center_point = getCenterPoint();
    int blocksize = 3;

    auto iter = pp_.begin();
    auto iter_next = iter + 1;

    primitive::Point p1 = **iter;
    primitive::Point p2 = **iter_next;

    Skeleton skeleton{renderer_, cg_};
    while (iter_next != pp_.end()){
        p1 = **iter;
        p2 = **iter_next;
        skeleton.update(0.0, primitive::Line{p1, p2}.length(), center_point,
                        primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1, blocksize, false, true);
        ++iter;
        ++iter_next;
        // break;
    }
    iter_next = pp_.begin();
    p1 = **iter;
    p2 = **iter_next;
    skeleton.update(0.0, primitive::Line{p1, p2}.length(), center_point,
                    primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1, blocksize, false, true);
}

void Asteroid::display()
{
    auto iter = pp_.begin();
    primitive::Point *p1;
    primitive::Point *p2;
    figure::FactoryShape factory{renderer_};
    factory.color({0, 0, 255, 255});
    for (; iter != pp_.end()-1; ++iter){
        p1 = *iter;
        p2 = *(iter+1);
        factory.line(*p1, *p2).draw();
    }
    p1 = *pp_.begin();
    factory.line(*p1, *p2).draw();
    fill();
}

void Asteroid::changePosition(primitive::Direction direction_xy){
    auto tmp = getCenterPoint();
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        primitive::Direction tmpDirection {movement->getOffset().x + direction_xy.x, movement->getOffset().y + direction_xy.y};
        (*iter)->move(tmpDirection);
    }
}

void Asteroid::rotate(){
    auto tmp = getCenterPoint();
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        (*iter)->rotate(tmp, movement->angularShift_);
    }
}

void Asteroid::changePosition(Star *star){
    auto star_mass {star->getMass()};
    auto star_center {star->getCoordinates()};
    
    auto tmp = getCenterPoint();

    auto distance {primitive::Line{star_center, tmp}.length()};
    auto F = (space::gravitationalConstant * getMass() * star_mass) / distance;
    auto x_shift = F / abs(abs(star_center.x) - abs(tmp.x));
    auto y_shift = F / abs(abs(star_center.y) - abs(tmp.y));
    
    if (tmp.x > star_center.x) { x_shift = -x_shift; }
    if (tmp.y > star_center.y) { y_shift = -y_shift; }
    changePosition(primitive::Direction{x_shift, y_shift}); 
}

primitive::Point* Asteroid::getFirstPoint(){
    return pp_[0];
}

Star::Star(SDL_Renderer* renderer, primitive::Point coordinate, double radius): SpaceObject(renderer, coordinate), radius_{radius}{
    rotation_ = 1.0;
}

double Star::getArea(){
    return M_PI * pow(radius_, 2);
}

double Star::getMass(){
    return pow(getArea(), 2);
}

void Star::changePosition(primitive::Direction direction_xy){
    coordinate_.x += direction_xy.x;
    coordinate_.y += direction_xy.y;
    rotation_ += radius_ / 1000000;
    display_delay_ = primitive::delay(kDisplayDelay);
}

void Star::display() {
    double angle_start      { 0.0 };
    double angle_finish     { 360.0 };
    double angle_step       { M_PI / 10 };
    int number_of_layers    { 10 };

    figure::FactoryShape factory{renderer_};
    factory.color({255, 255, 0, 255});

    for (double r = radius_; r > 0; r -= radius_ / number_of_layers ){

        auto p_start_ = primitive::Point{coordinate_.x - r, coordinate_.y - r};
        p_start_.rotate(coordinate_, rotation_);
        auto p_tmp_ = p_start_;
        auto p_ = primitive::Point{0.0};
        
        for (double angle = angle_step; angle <= angle_finish; angle += angle_step) {
            auto block_size = 10;
            p_ = primitive::Point{coordinate_.x - r, coordinate_.y - r};
            p_.rotate(coordinate_, angle + rotation_);
            factory.line(p_tmp_, p_).draw();
            p_tmp_ = p_;
        }
    }
}
