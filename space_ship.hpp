#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>

#include <SDL2/SDL.h>

#include "colorschema.hpp"
#include "primitive/size.hpp"
#include "speed.hpp"

struct Nozzle{
    std::vector <Point> points;
    std::vector <Point> origin_points;
    SDL_Renderer* renderer;
    ColorSchema *cs;
    Speed *speed;

    Nozzle(SDL_Renderer* renderer, Point a, Point b, Point c, Speed *speed){
        origin_points.push_back(a);
        origin_points.push_back(b);
        origin_points.push_back(c);
        copy(origin_points.begin(), origin_points.end(), back_inserter(points));
        this->renderer = renderer;
        this->speed = speed;
        cs = new ColorSchema(Color(255, 17, 0), Color(255, 237, 0));
    }
    void display(){
        cs->update(speed->get_current_a());
        SDL_SetRenderDrawColor(renderer, cs->get_r(), cs->get_g(), cs->get_b(), 255);
        SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[1].x, points[1].y);
        SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[2].x, points[2].y);
        SDL_RenderDrawLine(renderer, points[1].x, points[1].y, points[2].x, points[2].y);
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
    std::chrono::time_point<std::chrono::system_clock> display_delay_;
    std::chrono::time_point<std::chrono::system_clock> rotation_delay_;
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
    std::chrono::time_point<std::chrono::system_clock> life_time_;

public:
    Projectile(SDL_Renderer*, primitive::Size, int, int, int, int);
    void changePosition(DirectionXY);
    void display(bool);
    Point* getXY();
    std::pair<Point, Point> getLine();
    std::chrono::time_point<std::chrono::system_clock> get_life_time();
    ~Projectile();
};

class SpaceShip: public SpaceObject{
private:
    ColorSchema *cs_;
    std::vector<Point> skeleton_;
    std::chrono::time_point<std::chrono::system_clock> shoot_delay_;
    std::chrono::time_point<std::chrono::system_clock> ship_color_change_;
    Point initial_median_intersection_;
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
    std::vector<Point> pp;
    void updateNozzles();
    double getCurrentA();
    Point getMedianIntersaction();
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
    std::vector<Point*> pp_;
    void fill();
public:
    ColorGeneratorAsteroid *cg;
    Point getCenterPoint();
    void display(bool);
    std::vector<Point*>& get_points();
    Asteroid(SDL_Renderer*, primitive::Size, int, int);
    void changePosition(DirectionXY);
    Point* getFirstPoint();
    ~Asteroid();
};

#endif
