#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "SpaceShip.hpp"
#include "common.hpp"

class Fragment{
    int dots_number;
    Point initial_p;
    vector<Point> dots;
    Fragment(Point, int);
};

class Explosion{
    Point p;
public:
    Explosion(Point p);
    void bang();
};

#endif // EXPLOSION_H
