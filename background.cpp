#include "background.hpp"

#include <SDL2/SDL.h>

constexpr auto kCoef = 5;  // TODO: Is it coefficient?
constexpr auto kCitizenSize = 5 * kCoef;

void Background::fillBackground(){
    int gradient_size = 1;
    int steps = 1 + (screen_size_.width / (kCitizenSize * gradient_size));

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = screen_size_.width;
    rect.h = kCitizenSize * 10;

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

    while (rect.y < screen_size_.height) {
        r = (r_start < r_finish) ? r + r_step : r - r_step;
        g = (g_start < g_finish) ? g + g_step : g - g_step;
        b = (b_start < b_finish) ? b + b_step : b - b_step;

        SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
        SDL_RenderFillRect(renderer_, &rect);
        rect.x = 0;
        rect.y += kCitizenSize * gradient_size;
    }
}

void Background::drawGrid(){
    SDL_SetRenderDrawColor(renderer_, 192, 192, 192, 255);

    for (int i = 0; i < screen_size_.height; i = i + kCitizenSize)
        SDL_RenderDrawLine(renderer_, 0, i, screen_size_.width, i);

    for (int i = 0; i < screen_size_.width; i = i + kCitizenSize)
        SDL_RenderDrawLine(renderer_, i, 0, i, screen_size_.height);
}
