#include "polygon.hpp"

#include <SDL2/SDL.h>

#include "exception.hpp"

namespace figure {

void Polygon::drawLine(int x1, int y1, int x2, int y2) const
{
  if (SDL_RenderDrawLine(renderer_, x1, y1, x2, y2) == -1) {
      throw ErrorDrawing{};
  }
}

void Polygon::draw() const
{
  for (auto i = 0; i < points_.size() - 1; ++i) {
    const auto& [x1, y1] = points_.at(i);
    const auto& [x2, y2] = points_.at(i+1);
    drawLine(x1, y1, x2, y2);
  }
  const auto& [x1, y1] = points_.at(points_.size() - 1);
  const auto& [x2, y2] = points_.at(0);
  drawLine(x1, y1, x2, y2);
}
}  // namespace figure
