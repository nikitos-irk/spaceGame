#include "rectangle.hpp"

#include <SDL2/SDL.h>

#include "exception.hpp"

namespace figure
{
void Rectangle::fill() const
{
  if (SDL_RenderFillRect(renderer_, &rect_) == -1) {
      throw ErrorDrawing{};
  }
}
}  // namespace figure
