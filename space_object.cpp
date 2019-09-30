#include "space_object.hpp"

#include <cstdlib>

#include "skeleton.hpp"

#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/line.hpp"

constexpr auto kDisplayDelay = 10ms;

bool Asteroid::isAlive(){ return alive_; }
void Asteroid::markAsDead() { alive_ = false; }

Asteroid::Asteroid(SDL_Renderer *renderer, primitive::Point coordinate)
    : renderer_{renderer},
      coordinate_{coordinate},
      cg_{{160, 177, 188}, {119, 98, 84}, {128, 0, 0}, {81, 81, 81},
          {76, 62, 54}, {139, 69, 19}}
{
    display_delay_ = primitive::delay(kDisplayDelay);
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
