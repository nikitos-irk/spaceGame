#ifndef FIGURE_FACTORY_SHAPE_HPP_
#define FIGURE_FACTORY_SHAPE_HPP_

#include "line.hpp"
#include "point.hpp"
#include "rectangle.hpp"
#include "polygon.hpp"

struct SDL_Renderer;

namespace primitive {
struct Color;
struct Line;
struct Point;
class Size;
}  // namespace primitive

namespace figure
{
class FactoryShape
{   
public:
    explicit FactoryShape(SDL_Renderer* renderer);
    FactoryShape& color(primitive::Color const& color);
    Point point(primitive::Point const& point) const;
    Line line(primitive::Point const& begin, primitive::Point const& end) const;
    Line line(primitive::Line const& line) const;
    Rectangle rectangle(primitive::Point const& point,
                        primitive::Size const& size) const;
    Polygon polygon(std::vector<primitive::Point> const& points) const;

private:
    SDL_Renderer* renderer_;
};
}  // namespace figure

#endif /* FIGURE_FACTORY_SHAPE_HPP_ */
