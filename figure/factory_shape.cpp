#include "factory_shape.hpp"

#include "exception.hpp"
#include "primitive/point.hpp"

namespace figure
{

FactoryShape::FactoryShape(SDL_Renderer *renderer)
    : renderer_{renderer}
{
    if (!renderer_) throw BadRenderer{};
}

Point FactoryShape::point(primitive::Point const& point) const
{
    return Point{renderer_, static_cast<int>(point.x), static_cast<int>(point.y)};
}

Line FactoryShape::line(primitive::Point const& begin,
                        primitive::Point const& end) const
{
    return Line{renderer_,
        static_cast<int>(begin.x), static_cast<int>(begin.y),
        static_cast<int>(end.x), static_cast<int>(end.y)
    };
}
}  // namespace figure

