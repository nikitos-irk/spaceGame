#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "primitive/size.hpp"

struct SDL_Renderer;

class Background{
private:
    SDL_Renderer *renderer_;
    primitive::Size screen_size_;

public:
    Background(SDL_Renderer* renderer, primitive::Size size)
        : renderer_{renderer},
          screen_size_{size} {}
    void fillBackground();
    void drawGrid();
};

#endif
