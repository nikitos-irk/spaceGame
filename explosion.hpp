#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "SDL2/SDL.h"
#include "common.hpp"
#include <functional>
#include <vector>
#include <iostream>

#define FRAGMENT_SHIFT_DELAY 150

class Explosion{
    Point p;
    SDL_Renderer *renderer;
    class Fragment{
        std::chrono::time_point<std::chrono::system_clock> fragment_shift_delay;
        int dots_number;
        double x_shift;
        double y_shift;
        Point initial_p;
        Point next_p;
        vector<Point> dots;
    public:
        void display(SDL_Renderer *);
        void shift();
        Fragment(Point, Point, int);
    };
    vector<Fragment> fragments;
public:
    Explosion(Point p, SDL_Renderer*);
    void bang();
    void display();
};

#endif // EXPLOSION_HPP
