#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>

#include "colorschema.hpp"
#include "speed.hpp"

#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "primitive/time.hpp"
#include "space/gravity.hpp"

struct SDL_Renderer;

class Star;
namespace space { class Gravity; }

class SpaceObject{
protected:
    SDL_Renderer *renderer_{nullptr};
    primitive::Point coordinate_;
    primitive::Time display_delay_;
    primitive::Time rotation_delay_;
    bool alive_{true};

public:
    SpaceObject(SDL_Renderer*, primitive::Point coordinate);
    virtual void display() = 0;
    virtual void changePosition(primitive::Direction) = 0;
    virtual ~SpaceObject();
    bool isAlive();
    void markAsDead();
};

class Asteroid: public SpaceObject {
private:
    std::vector<primitive::Point*> pp_;
    void fill();
    double getTriangleArea(primitive::Point *a, primitive::Point *b);
    primitive::Point centralPoint;
    double mass;
    bool massIsReady {false};
    primitive::Movement *movement;
public:
    ColorGenerator cg_;
    primitive::Point getCenterPoint();
    void display() override;
    std::vector<primitive::Point*>& get_points();
    Asteroid(SDL_Renderer*, primitive::Point coordinate);
    void changePosition(primitive::Direction) override;
    void rotate();
    void changePosition(Star*);
    primitive::Point* getFirstPoint();
    double getArea();
    double getMass();
    ~Asteroid();
    space::Gravity *gravity_;
};

class Star: public SpaceObject {
private:
    double radius_;
    double rotation_;

public:
    Star(SDL_Renderer*, primitive::Point, double);
    void display() override;
    void changePosition(primitive::Direction) override;
    double getArea();
    double getMass();
    primitive::Point getCoordinates() const {return coordinate_;};
};

#endif
