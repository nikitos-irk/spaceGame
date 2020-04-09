#include "sdl_scene.hpp"

#include "colorschema.hpp"
#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "skeleton.hpp"
#include "space/background.hpp"
#include "space/grid.hpp"
#include "space/life_amount.hpp"
#include "space/projectile.hpp"
#include "space/nozzle.hpp"
#include "space/ship.hpp"

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
    figure::FactoryShape factory{renderer_};
    const auto& border = ship.get_border();
    factory.color(ship.colors.get_color()).polygon(border).draw();

    Skeleton skeleton{renderer_, ColorGenerator{{220,220,220}, {192,192,192},
                                                {105,105,105}, {211,211,211},
                                                {119,136,153}}};
    skeleton.update(ship.getTiltAngel(), size_.width, border[0],
                   primitive::Point{border[1].x/2 + border[2].x/2,
                                    border[1].y/2 + border[2].y/2},
                   border[2], 4, true, true);
    this->draw(ship.get_left_nozzle());
    this->draw(ship.get_right_nozzle());
}

void SdlScene::draw(space::Nozzle const& nozzle)
{
    figure::FactoryShape factory{renderer_};
    const auto& border = nozzle.get_border();
    factory.color(nozzle.colors.get_color()).polygon(border).draw();

    Skeleton skeleton{renderer_, ColorGenerator{{220,220,220}, {192,192,192},
                                                {105,105,105}, {211,211,211},
                                                {119,136,153}}};
    skeleton.update(nozzle.getTiltAngel(), size_.width, border[0],
                    primitive::Point{border[1].x/2 + border[2].x/2,
                                     border[1].y/2 + border[2].y/2},
                    border[2], 1, true, false);
}

void SdlScene::draw(space::Projectile const& projectile)
{
}

void SdlScene::draw(space::LifeAmount const& lifes)
{
  figure::FactoryShape factory{renderer_};
  factory.color({0, 255, 0, 255})
      .rectangle({5.0, double(size_.height - 32)}, lifes.kSize).fill();

  factory.color({255, 0, 0, 255});
  for (int i = 0; i < lifes.get_amount(); ++i) {
      factory.rectangle({10.0 + i * 15, double(size_.height - 30)},
                        lifes.kBarSize).fill();
  }
}

}  // namespace scene

