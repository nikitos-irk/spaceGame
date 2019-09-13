#include <cstdlib>
#include <ctime>

#include <iostream>

#include "game.hpp"

constexpr int kScreenWidth = 640;
constexpr int kScreenHeight = 480;
constexpr primitive::Size kScreenSize{kScreenWidth, kScreenHeight};

void logSDLError(std::ostream &os, const std::string &msg){
os << msg << " error: " << SDL_GetError() << std::endl;
}

int main( int argc, char* args[] )
{
    std::srand(unsigned(std::time(0)));

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout, "SDL_Init Error: ");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kScreenSize.width, kScreenSize.height, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        SDL_Quit();
        return 1;
    }

    Game *space_game = new Game(renderer, kScreenSize, 5);
    space_game->run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
