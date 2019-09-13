#include "background.hpp"

#include <SDL2/SDL.h>

#include "figure/factory_shape.hpp"
#include "primitive/point.hpp"

constexpr auto kCoef = 5;  // TODO: Is it coefficient?
constexpr auto kCitizenSize = 5 * kCoef;

void Background::fillBackground(){
    int gradient_size = 1;
    int steps = 1 + (screen_size_.width / (kCitizenSize * gradient_size));

    double r_start = 100;
    double g_start = 100;
    double b_start = 100;

    double r_finish = 0;
    double g_finish = 0;
    double b_finish = 60;

    double r = r_start;
    double g = g_start;
    double b = b_start;

    double r_step = abs(r_finish - r_start) / steps;
    double g_step = abs(g_finish - g_start) / steps;
    double b_step = abs(b_finish - b_start) / steps;

    primitive::Point point{0.0, 0.0};
    primitive::Size size{screen_size_.width, kCitizenSize * 10};
    figure::FactoryShape factory{renderer_};
    while (point.y < screen_size_.height) {
        r = (r_start < r_finish) ? r + r_step : r - r_step;
        g = (g_start < g_finish) ? g + g_step : g - g_step;
        b = (b_start < b_finish) ? b + b_step : b - b_step;

        SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
        factory.rectangle(point, size).fill();
        point.x = 0.0;
        point.y += kCitizenSize * gradient_size;
    }
}

void Background::drawGrid(){
    SDL_SetRenderDrawColor(renderer_, 192, 192, 192, 255);

    figure::FactoryShape factory{renderer_};
    for (int i = 0; i < screen_size_.height; i = i + kCitizenSize) {
        factory.line({0.0, double(i)},
                     {double(screen_size_.width), double(i)}).draw();
    }

    for (int i = 0; i < screen_size_.width; i = i + kCitizenSize) {
        factory.line({double(i), 0.0},
                     {double(i), double(screen_size_.height)}).draw();
    }
}
