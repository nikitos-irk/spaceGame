#include "line.hpp"

#include "exception.hpp"

#include <SDL2/SDL.h>

namespace figure
{
void Line::draw() const
{
    if (SDL_RenderDrawLine(renderer_, x1_, y1_, x2_, y2_) == -1) {
        throw ErrorDrawing{};
    }
}
}  // namespace figure
