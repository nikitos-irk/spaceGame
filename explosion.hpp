#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include <chrono>
#include <vector>

#include "common.hpp"
#include "primitive/point.hpp"

struct SDL_Renderer;

class Asteroid;

class Explosion{
public:
    Asteroid* ast;
private:
    primitive::Point p_;
    SDL_Renderer *renderer_;
    std::chrono::time_point<std::chrono::system_clock> destroy_time_;
    class Fragment{
        std::chrono::time_point<std::chrono::system_clock> fragment_shift_delay_;
        int dots_number_;
        double x_shift_;
        double y_shift_;
        primitive::Point initial_p_;
        primitive::Point next_p_;
        double angle_;
    public:
        std::vector<primitive::Point> dots;
        void display(SDL_Renderer *, bool);
        void shift();
        Fragment(Asteroid*, primitive::Point, primitive::Point, int);
        Fragment(Asteroid*, primitive::Point, primitive::Point, primitive::Point);
        Asteroid *ast;
    };
    std::vector<Fragment> fragments_;
public:
    Explosion(primitive::Point p, SDL_Renderer*, Asteroid*);
    void shift(DirectionXY);
    void display(bool);
    bool isAlive();
    ~Explosion();
};

#endif // EXPLOSION_HPP
