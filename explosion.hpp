#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "SDL2/SDL.h"
#include "common.hpp"
#include <functional>
#include <vector>
#include <iostream>
#include "SpaceShip.hpp"

#define FRAGMENT_SHIFT_DELAY 50
#define EXPLOSION_LIFE_TIME 1000

class Explosion{
public:
    Asteroid* ast;
private:
    Point p;
    SDL_Renderer *renderer;
    std::chrono::time_point<std::chrono::system_clock> destroy_time;
    class Fragment{
        std::chrono::time_point<std::chrono::system_clock> fragment_shift_delay;
        int dots_number;
        double x_shift;
        double y_shift;
        Point initial_p;
        Point next_p;
        double angle;
    public:
        vector<Point> dots;
        void display(SDL_Renderer *, bool);
        void shift();
        Fragment(Asteroid*, Point, Point, int);
        Fragment(Asteroid*, Point, Point, Point);
        Asteroid *ast;
    };
    vector<Fragment> fragments;
public:
    Explosion(Point p, SDL_Renderer*, Asteroid*);
    void shift(DirectionXY);
    void display(bool);
    bool isAlive();
    ~Explosion();
};

#endif // EXPLOSION_HPP
