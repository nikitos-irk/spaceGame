#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "primitive/size.hpp"

struct SDL_Renderer;

// Hope I will think something more interesting background than it is now
class Background{
public:
    Background(SDL_Renderer* renderer, primitive::Size size, bool grid = false)
        : renderer_{renderer}, screen_size_{size}, grid_{grid} {}
    void fill();

private:
    void drawGrid();
    SDL_Renderer *renderer_;
    primitive::Size screen_size_;
    bool grid_{false};
};

#endif
