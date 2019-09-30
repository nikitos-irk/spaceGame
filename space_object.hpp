#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>

#include "colorschema.hpp"

#include "primitive/point.hpp"
#include "primitive/time.hpp"

struct SDL_Renderer;

class Asteroid {
private:
    void fill();
    std::vector<primitive::Point*> pp_;
    bool alive_{true};
    SDL_Renderer *renderer_{nullptr};
    primitive::Point coordinate_;
    primitive::Time display_delay_;
    primitive::Time rotation_delay_;
public:
    Asteroid(SDL_Renderer*, primitive::Point coordinate);
    bool isAlive();
    void markAsDead();
    void display();
    void changePosition(primitive::Direction);
    primitive::Point getCenterPoint();
    std::vector<primitive::Point*>& get_points();
    primitive::Point* getFirstPoint();
    ColorGenerator cg_;
};

#endif
