#include "factory_shape.hpp"

#include <SDL2/SDL.h>

#include "exception.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"

namespace figure
{

FactoryShape::FactoryShape(SDL_Renderer *renderer)
    : renderer_{renderer}
{
    if (!renderer_) throw BadRenderer{};
}

Point FactoryShape::point(primitive::Point const& point) const
{
    return {renderer_, static_cast<int>(point.x), static_cast<int>(point.y)};
}

Line FactoryShape::line(primitive::Point const& begin,
                        primitive::Point const& end) const
{
    return {renderer_,
        static_cast<int>(begin.x), static_cast<int>(begin.y),
        static_cast<int>(end.x), static_cast<int>(end.y)
    };
}

Rectangle FactoryShape::rectangle(primitive::Point const& point,
                                  primitive::Size const& size) const
{
    return {renderer_, {
        static_cast<int>(point.x), static_cast<int>(point.y),
        size.width, size.height
    }};
}

FactoryShape& FactoryShape::color(primitive::Color const& color)
{
    if (SDL_SetRenderDrawColor(renderer_, color.red, color.green, color.blue,
                               color.alpha) == -1) {
        throw ErrorDrawing{};
    }
    return *this;
}

}  // namespace figure

