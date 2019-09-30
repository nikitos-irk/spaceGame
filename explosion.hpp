#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include <vector>

#include "colorschema.hpp"

#include "primitive/point.hpp"
#include "primitive/time.hpp"

struct SDL_Renderer;

class Asteroid;

class Explosion {
private:
    primitive::Point p_;
    SDL_Renderer *renderer_;
    primitive::Time destroy_time_;
    class Fragment {
        primitive::Time fragment_shift_delay_;
        int dots_number_;
        double x_shift_;
        double y_shift_;
        primitive::Point next_p_;
        double angle_;
    public:
        std::vector<primitive::Point> dots;
        void display(SDL_Renderer *);
        void shift();
        Fragment(ColorGenerator cg, primitive::Point, primitive::Point, int);
        Fragment(ColorGenerator cg, primitive::Point, primitive::Point, primitive::Point);
        ColorGenerator cg_;
    };
    std::vector<Fragment> fragments_;
public:
    Explosion(primitive::Point p, SDL_Renderer*, Asteroid*);
    void shift(primitive::Direction);
    void display();
    bool isAlive();
};

#endif // EXPLOSION_HPP
