#ifndef FIGURE_FACTORY_SHAPE_HPP_
#define FIGURE_FACTORY_SHAPE_HPP_

#include "point.hpp"

struct SDL_Renderer;

namespace primitive {
class Point;
}  // namespace primitive

namespace figure
{
class FactoryShape
{
public:
    explicit FactoryShape(SDL_Renderer* renderer) : renderer_{renderer} {}
    Point point(primitive::Point const& point) const;
private:
    SDL_Renderer* renderer_;
};
}  // namespace figure

#endif /* FIGURE_FACTORY_SHAPE_HPP_ */
