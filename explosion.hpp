#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "SDL2/SDL.h"
#include "common.hpp"
#include <functional>
#include <vector>
#include <iostream>

#define FRAGMENT_SHIFT_DELAY 50
#define EXPLOSION_LIFE_TIME 1000

class Explosion{
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
    public:
        vector<Point> dots;
        void display(SDL_Renderer *);
        void shift();
        Fragment(Point, Point, int);
    };
    vector<Fragment> fragments;
public:
    Explosion(Point p, SDL_Renderer*);
    void shift(DirectionXY);
    void display();
    bool isAlive();
};

#endif // EXPLOSION_HPP
