#include "point.hpp"

#include "exception.hpp"

#include <SDL2/SDL.h>

namespace figure
{
void Point::draw() const
{
    if (SDL_RenderDrawPoint(renderer_, x_, y_) == -1) {
        throw ErrorDrawing{};
    }
}

}  // namespace figure
