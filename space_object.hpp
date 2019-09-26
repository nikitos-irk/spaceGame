#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>

#include "colorschema.hpp"
#include "speed.hpp"

#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "primitive/time.hpp"

struct SDL_Renderer;

class SpaceObject{
protected:
    SDL_Renderer *renderer_{nullptr};
    primitive::Point coordinate_;
    primitive::Size screen_size_;
    primitive::Time display_delay_;
    primitive::Time rotation_delay_;
    bool alive_{true};

public:
    SpaceObject(SDL_Renderer*, primitive::Size, primitive::Point coordinate);
    virtual void display() = 0;
    virtual void changePosition(primitive::Direction) = 0;
    virtual ~SpaceObject();
    bool isAlive();
    void markAsDead();
};

class Projectile: public SpaceObject{
private:
    primitive::Direction direction_;
    primitive::Point previous_;
    primitive::Time life_time_;

public:
    Projectile(SDL_Renderer*, primitive::Size, primitive::Direction,
        primitive::Point);
    void changePosition(primitive::Direction) override;
    void display() override;
    primitive::Point* getXY();
    std::pair<primitive::Point, primitive::Point> getLine();
    primitive::Time get_life_time();
    ~Projectile();
};

class Asteroid: public SpaceObject {
private:
    std::vector<primitive::Point*> pp_;
    void fill();
public:
    ColorGenerator cg_;
    primitive::Point getCenterPoint();
    void display() override;
    std::vector<primitive::Point*>& get_points();
    Asteroid(SDL_Renderer*, primitive::Size, primitive::Point coordinate);
    void changePosition(primitive::Direction) override;
    primitive::Point* getFirstPoint();
    ~Asteroid();
};

#endif
