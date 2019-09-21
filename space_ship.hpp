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
    SDL_Renderer* renderer{nullptr};
    ColorSchema* cs{nullptr};
    Speed* speed{nullptr};

    Nozzle(SDL_Renderer* renderer, primitive::Point a, primitive::Point b,
           primitive::Point c, Speed *speed)
        : points{a, b, c},
          origin_points{a, b, c},
          renderer{renderer},
          cs{new ColorSchema{primitive::Color{255, 17, 0},
                             primitive::Color{255, 237, 0}}},
          speed{speed} {}

    void display(){
        cs->update(speed->get_current_a());
        figure::FactoryShape factory{renderer};
        factory.color(cs->get_color());
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
    primitive::Direction getDirection();
    primitive::Direction getOffset();
    void display() override;
    void changeX(bool);
    void changeY(bool);
    void changePosition(primitive::Direction) override {}
    ~SpaceShip();
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
