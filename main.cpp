#include "SDL2/SDL.h"
#include "iostream"
#include "vector"
#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <map>
#include <utility>
#include <functional>
#include <random>
#include <list>
#include <set>
#include "SpaceShip.hpp"

const int COEF = 5;
const int CITIZEN_SIZE = 5 * COEF;
const int CITIZEN_STEP = 5 * COEF;

using namespace std;

void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

void fill_background(SDL_Renderer *renderer){
	int gradient_size = 1;
	int steps = 1 + (SCREEN_HEIGHT / (CITIZEN_SIZE * gradient_size));

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = CITIZEN_SIZE * 10;

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
	
	while (rect.y < SCREEN_HEIGHT) {
		r = (r_start < r_finish) ? r + r_step : r - r_step;
		g = (g_start < g_finish) ? g + g_step : g - g_step;
		b = (b_start < b_finish) ? b + b_step : b - b_step;

		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderFillRect(renderer, &rect);
		rect.x = 0;
		rect.y += CITIZEN_SIZE * gradient_size;
	}
}

void draw_grid(SDL_Renderer* renderer){
	
	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	
	for (int i = 0; i < SCREEN_HEIGHT; i = i + CITIZEN_SIZE)
		SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH, i);

	for (int i = 0; i < SCREEN_WIDTH; i = i + CITIZEN_SIZE)
		SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
}

void displayObjects(vector<SpaceObject*> &spaceObjects){
	for (auto spaceObject = spaceObjects.begin(); spaceObject != spaceObjects.end(); ++spaceObject){
		(*spaceObject)->display();
	}
}

void changeObjectsPositions(vector<SpaceObject*> &spaceObjects, bool direction){
	auto ship = spaceObjects.begin();
	DirectionXY directionXY = (dynamic_cast<SpaceShip*> (*ship))->get_direction();
	for (auto spaceObject = spaceObjects.begin() + 1; spaceObject != spaceObjects.end(); ++spaceObject){
		if (direction){ (*spaceObject)->change_position(directionXY); }
		else{
			directionXY.x *= -1;
			directionXY.y *= -1;
			(*spaceObject)->change_position(directionXY);
		}
	}
}

int main( int argc, char* args[] )
{
	srand(unsigned(std::time(0)));
	vector<SpaceObject*> spaceObjects;

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init Error: ");
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		// cleanup(window);
		SDL_Quit();
		return 1;
	}
	SpaceShip *my_ship = new SpaceShip(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

    SDL_Event e;
	int quit = 1;

	spaceObjects.push_back(my_ship);
	for (int i = 0; i < 10; ++i){
		int tmp_x = rand() % SCREEN_WIDTH;
		int tmp_y = rand() % SCREEN_HEIGHT;
		int direction_x = 5 - rand() % 10;
		int direction_y = 5 - rand() % 10;
		spaceObjects.push_back(new Asteroid(renderer, tmp_x, tmp_y, direction_x, direction_y));
	}

	fill_background(renderer);
	displayObjects(spaceObjects);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	while(quit) {
		while( SDL_PollEvent( &e ) != 0 ) {
			if( e.type == SDL_QUIT ){
				quit = 0;
			} else if (e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym){
			    	case SDL_QUIT:
						quit = 0;
						break;
					case SDLK_UP:
						changeObjectsPositions(spaceObjects, true);
						break;
					case SDLK_DOWN:
						changeObjectsPositions(spaceObjects, false);
						break;
					case SDLK_LEFT:
						my_ship->change_x(false);
						break;
					case SDLK_RIGHT:
						my_ship->change_x(true);
						break;
					case SDLK_SPACE:
						spaceObjects.push_back(my_ship->shoot());
						break;
					default:
						break;
					}
				}
		}
		fill_background(renderer);
		displayObjects(spaceObjects);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}