#ifndef FIGURE_FACTORY_SHAPE_HPP_
#define FIGURE_FACTORY_SHAPE_HPP_

#include "line.hpp"
#include "point.hpp"

struct SDL_Renderer;

namespace primitive {
class Line;
class Point;
}  // namespace primitive

namespace figure
{
class FactoryShape
{
public:
    explicit FactoryShape(SDL_Renderer* renderer);
    Point point(primitive::Point const& point) const;
    Line line(primitive::Point const& begin, primitive::Point const& end) const;

private:
    SDL_Renderer* renderer_;
};
}  // namespace figure

#endif /* FIGURE_FACTORY_SHAPE_HPP_ */
