#include "point.hpp"

#include <SDL2/SDL.h>

#include "error_drawing.hpp"

namespace figure
{
void Point::draw() const
{
  if (SDL_RenderDrawPoint(renderer_, x_, y_) == -1) {
      throw ErrorDrawing{};
  }
}

}  // namespace figure
