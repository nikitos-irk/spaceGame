#include "sdl_scene.hpp"

#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "space/background.hpp"

namespace scene {

void SdlScene::draw(space::Background* background) {
    constexpr auto kCitizenSize = 25;

    int steps = (size_.width / kCitizenSize) + 1;

    auto start = background->gradient_start;
    auto finish = background->gradient_finish;

    int r_step = std::abs(finish.red - start.red) / steps;
    int g_step = std::abs(finish.green - start.green) / steps;
    int b_step = std::abs(finish.blue - start.blue) / steps;

    primitive::Point point{0.0, 0.0};
    primitive::Size size{size_.width, kCitizenSize};
    primitive::Color color = start;
    figure::FactoryShape factory{renderer_};
    while (point.y < size_.height) {
        color.red += (start.red < finish.red) ? r_step : - r_step;
        color.green += (start.green < finish.green) ? g_step : - g_step;
        color.blue += (start.blue < finish.blue) ? b_step : - b_step;

        factory.color(color).rectangle(point, size).fill();
        point.move({0.0, kCitizenSize});
    }
    if (background->grid) {
      factory.color(background->grid_color);
      for (int i = 0; i < size_.height; i = i + kCitizenSize) {
          factory.line({0.0, double(i)},
                       {double(size_.width), double(i)}).draw();
      }

      for (int i = 0; i < size_.width; i = i + kCitizenSize) {
          factory.line({double(i), 0.0},
                       {double(i), double(size_.height)}).draw();
      }
    }
}

void SdlScene::draw(space::Ship* ship) {
}

}  // namespace scene
