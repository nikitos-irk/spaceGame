#include "sdl_scene.hpp"

#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "space/background.hpp"
#include "space/grid.hpp"

namespace scene {

void SdlScene::draw(space::Background const& background)
{
    constexpr auto kCitizenSize = 25;

    const auto& start = background.gradient_start;
    const auto& finish = background.gradient_finish;

    int steps = (size_.width / kCitizenSize) + 1;
    int r_step = (finish.red - start.red) / steps;
    int g_step = (finish.green - start.green) / steps;
    int b_step = (finish.blue - start.blue) / steps;

    primitive::Color color = start;
    figure::FactoryShape factory{renderer_};
    for (auto i = 0; i < steps; ++i) {
        color.red += r_step;
        color.green += g_step;
        color.blue += b_step;

        factory.color(color)
               .rectangle({0.0, double(kCitizenSize * i)},
                          {size_.width, kCitizenSize})
               .fill();
    }
    if (background.grid) this->draw(*background.grid);
}

void SdlScene::draw(space::Grid const& grid)
{
    figure::FactoryShape factory{renderer_};
    factory.color(grid.color);
    auto i{0};
    while (i < size_.height || i < size_.width) {
        factory.line({0.0, double(i)},
                     {double(size_.width), double(i)}).draw();
        factory.line({double(i), 0.0},
                     {double(i), double(size_.height)}).draw();
        i += grid.cell_size;
    }
}

void SdlScene::draw(space::Ship const& ship)
{
}

}  // namespace scene
