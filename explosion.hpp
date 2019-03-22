#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "SDL2/SDL.h"
#include "SpaceShip.hpp"
#include "common.hpp"

#define FRAGMENT_SHIFT 10

class Explosion{
    Point p;
    SDL_Renderer *renderer;
    class Fragment{
        int dots_number;
        Point initial_p;
        Point next_p;
        vector<Point> dots;
    public:
        void show(SDL_Renderer *);
        void shift();
        Fragment(Point, Point, int);
    };
    vector<Fragment> fragments;
public:
    Explosion(Point p, SDL_Renderer*);
    void bang();
};

#endif // EXPLOSION_HPP
