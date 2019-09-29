#include "space_object.hpp"

#include <cstdlib>
#include <iostream>

#include "colorschema.hpp"
#include "skeleton.hpp"

#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/line.hpp"

constexpr auto kBlockSize = 5;
constexpr auto kDisplayDelay = 10ms;
constexpr auto kProjLifetime = 5s;

SpaceObject::SpaceObject(SDL_Renderer *renderer,
                         primitive::Point coordinate)
    : renderer_{renderer},
      coordinate_{coordinate}
{
  display_delay_ = primitive::delay(kDisplayDelay);
}

SpaceObject::~SpaceObject(){
    std::cout << "SpaceObject destructor" << std::endl;
}

bool SpaceObject::isAlive(){ return alive_; }
void SpaceObject::markAsDead() { alive_ = false; }

Projectile::Projectile(SDL_Renderer *renderer, primitive::Direction offset,
    primitive::Point coordinate)
    : SpaceObject(renderer, coordinate),
      direction_{offset} {
    life_time_ = primitive::delay(kProjLifetime);
}

primitive::Point* Projectile::getXY(){ return &coordinate_; }
std::pair<primitive::Point, primitive::Point> Projectile::getLine()
{ return std::make_pair(coordinate_, previous_); }

Projectile::~Projectile(){
    std::cout << "Projectile destructor" << std::endl;
}

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
}

std::vector<primitive::Point*>& Asteroid::get_points(){ return pp_; }

Asteroid::~Asteroid(){
    std::cout << "Asteroid destructor" << std::endl;
}

void out(double x, double y){
    std::cout << x << ":" << y << std::endl;
}

primitive::Time Projectile::get_life_time(){ return life_time_; }

void Projectile::display()
{
    if (display_delay_ > primitive::now()){ return; }

    double error = (double) - kBlockSize;
    double tmp_x = (double) kBlockSize - 0.5;
    double tmp_y = (double) 0.5;

    double cx = coordinate_.x - 0.5;
    double cy = coordinate_.y - 0.5;

    figure::FactoryShape factory{renderer_};
    factory.color({255, 255, 0, 255});
    while (tmp_x >= tmp_y){
        factory.point({cx + tmp_x, cy + tmp_y}).draw();
        factory.point({cx + tmp_y, cy + tmp_x}).draw();

        if (tmp_x != 0){
            factory.point({cx - tmp_x, cy + tmp_y}).draw();
            factory.point({cx + tmp_y, cy - tmp_x}).draw();
        }

        if (tmp_y != 0){
            factory.point({cx + tmp_x, cy - tmp_y}).draw();
            factory.point({cx - tmp_y, cy + tmp_x}).draw();
        }

        if (tmp_x != 0 && tmp_y != 0){
            factory.point({cx - tmp_x, cy - tmp_y}).draw();
            factory.point({cx - tmp_y, cy - tmp_x}).draw();
        }

        error += tmp_y;
        ++tmp_y;
        error += tmp_y;

        if (error >= 0){
            --tmp_x;
            error -= tmp_x;
            error -= tmp_x;
        }
    }

    factory.color({128, 0, 128, 255});
    for (double dy = 1; dy <= kBlockSize; dy += 1.0){
        double dx = floor(sqrt((2.0 * kBlockSize * dy) - (dy * dy)));
        double cx = coordinate_.x;
        double cy = coordinate_.y;
        factory.line({cx - dx, cy + dy - kBlockSize},
                     {cx + dx, cy + dy - kBlockSize}).draw();
        factory.line({cx - dx, cy - dy + kBlockSize},
                     {cx + dx, cy - dy + kBlockSize}).draw();
    }
    coordinate_.move(direction_);
    display_delay_ = primitive::delay(kDisplayDelay/5);
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
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        (*iter)->move(direction_xy);
    }
}

primitive::Point* Asteroid::getFirstPoint(){
    return pp_[0];
}

void Projectile::changePosition(primitive::Direction direction_xy){
    previous_ = coordinate_;
    coordinate_.move(direction_xy);
}
