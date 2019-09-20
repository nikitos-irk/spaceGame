#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>

#include "colorschema.hpp"
#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "primitive/time.hpp"
#include "speed.hpp"

struct Nozzle{
    std::vector <primitive::Point> points;
    std::vector <primitive::Point> origin_points;
    SDL_Renderer* renderer;
    ColorSchema *cs;
    Speed *speed;

    Nozzle(SDL_Renderer* renderer, primitive::Point a, primitive::Point b,
           primitive::Point c, Speed *speed) {
        origin_points.push_back(a);
        origin_points.push_back(b);
        origin_points.push_back(c);
        copy(origin_points.begin(), origin_points.end(), back_inserter(points));
        this->renderer = renderer;
        this->speed = speed;
        cs = new ColorSchema(primitive::Color{255, 17, 0}, primitive::Color{255, 237, 0});
    }
    void display(){
        cs->update(speed->get_current_a());
        figure::FactoryShape factory{renderer};
        factory.color({cs->get_r(), cs->get_g(), cs->get_b(), 255});
        factory.line(points[0], points[1]).draw();
        factory.line(points[0], points[2]).draw();
        factory.line(points[1], points[2]).draw();
    }


    void update(){
        double offsetLength = speed->get_current_a();
        points.clear();
        copy(origin_points.begin(), origin_points.end(), back_inserter(points));

        if (offsetLength < 0.001) { return; }

        double Cx = (points[0].x - points[1].x) * (offsetLength);
        double Cy = (points[0].y - points[1].y) * (offsetLength);

        double Cx2 = (points[1].x - points[2].x) * (offsetLength);
        double Cy2 = (points[1].y - points[2].y) * (offsetLength);

        points[0].x += Cx;
        points[0].y += Cy;

        points[2].x -= Cx2;
        points[2].y -= Cy2;
    }
};

class SpaceObject{
protected:
    SDL_Renderer *renderer_{nullptr};
    int x_, y_;
    primitive::Size screen_size_;
    primitive::Time display_delay_;
    primitive::Time rotation_delay_;
    bool alive_{true};

public:
    SpaceObject(SDL_Renderer*, primitive::Size, int, int);
    virtual void display(bool) = 0;
    virtual void changePosition(DirectionXY);
    virtual ~SpaceObject();
    bool isAlive();
    void markAsDead();
};

class Projectile: public SpaceObject{
private:
    int direction_x_;
    int direction_y_;
    double x_previous_;
    double y_previous_;
    primitive::Time life_time_;

public:
    Projectile(SDL_Renderer*, primitive::Size, int, int, int, int);
    void changePosition(DirectionXY);
    void display(bool);
    primitive::Point* getXY();
    std::pair<primitive::Point, primitive::Point> getLine();
    primitive::Time get_life_time();
    ~Projectile();
};

class SpaceShip: public SpaceObject{
private:
    ColorSchema *cs_;
    std::vector<primitive::Point> skeleton_;
    primitive::Time shoot_delay_;
    primitive::Time ship_color_change_;
    primitive::Point initial_median_intersection_;
    double getTiltAngel();
    double getLengthOfBase();
    primitive::Size space_size_;
    double nozzle_min_height_;
    double nozzle_max_height_;
    primitive::Size nozzle_size_;
    Nozzle *left_nozzle_;
    Nozzle *right_nozzle_;

public:
    ColorGeneratorShip *cg;
    std::vector<primitive::Point> pp;
    void updateNozzles();
    primitive::Point getMedianIntersaction();
    Speed *speed;
    void slowdown();
    void accelarate();
    void backwardSlowdown();
    void backwardAccelarate();
    SpaceShip(SDL_Renderer*, primitive::Size, int);
    Projectile * shoot();
    std::vector<Projectile*> projectiles;
    DirectionXY getDirection();
    DirectionXY getOffset();
    void display(bool);
    void changeX(bool);
    void changeY(bool);
    DirectionalVector getDerectionalVector();
    ~SpaceShip();
};

class Asteroid: public SpaceObject{
private:
    std::vector<primitive::Point*> pp_;
    void fill();
public:
    ColorGeneratorAsteroid *cg;
    primitive::Point getCenterPoint();
    void display(bool);
    std::vector<primitive::Point*>& get_points();
    Asteroid(SDL_Renderer*, primitive::Size, int, int);
    void changePosition(DirectionXY);
    primitive::Point* getFirstPoint();
    ~Asteroid();
};

#endif
