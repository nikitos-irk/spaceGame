#include "background.hpp"

#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"

constexpr auto kCoef = 5;  // TODO: Is it coefficient?
constexpr auto kCitizenSize = 5 * kCoef;

void Background::fillBackground(){
    constexpr int gradient_size = 1;
    int steps = 1 + (screen_size_.width / (kCitizenSize * gradient_size));

    constexpr double r_start = 100;
    constexpr double g_start = 100;
    constexpr double b_start = 100;

    constexpr double r_finish = 0;
    constexpr double g_finish = 0;
    constexpr double b_finish = 60;

    double r_step = abs(r_finish - r_start) / steps;
    double g_step = abs(g_finish - g_start) / steps;
    double b_step = abs(b_finish - b_start) / steps;

    primitive::Point point{0.0, 0.0};
    primitive::Size size{screen_size_.width, kCitizenSize * 10};
    primitive::Color color{100, 100, 100, 255};
    figure::FactoryShape factory{renderer_};
    while (point.y < screen_size_.height) {
        color.red += (r_start < r_finish) ? r_step : - r_step;
        color.green += (g_start < g_finish) ? g_step : - g_step;
        color.blue += (b_start < b_finish) ? b_step : - b_step;

        factory.color(color).rectangle(point, size).fill();
        point.x = 0.0;
        point.y += kCitizenSize * gradient_size;
    }
}

void Background::drawGrid(){
    figure::FactoryShape factory{renderer_};
    factory.color({192, 192, 192, 255});
    for (int i = 0; i < screen_size_.height; i = i + kCitizenSize) {
        factory.line({0.0, double(i)},
                     {double(screen_size_.width), double(i)}).draw();
    }

    for (int i = 0; i < screen_size_.width; i = i + kCitizenSize) {
        factory.line({double(i), 0.0},
                     {double(i), double(screen_size_.height)}).draw();
    }
}
