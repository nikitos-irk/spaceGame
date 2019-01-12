#ifndef HERBIVOR_H
#define HERBIVOR_H

#include "SDL2/SDL.h"
#include <utility>
#include <functional>
#include <vector>
#include <cmath>
#include <iostream>

#define BLOCK_SIZE 5
#define POINTS_COUNT 4
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;

static SDL_Point points[POINTS_COUNT] = {
    {SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
    {SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT/2 + SCREEN_HEIGHT / 5},
    {SCREEN_WIDTH/2 + 20, SCREEN_HEIGHT/2 + SCREEN_HEIGHT / 5},
    {SCREEN_WIDTH/2, SCREEN_HEIGHT/2}
};

class SpaceShip{
	
private:
	SDL_Renderer *renderer;
	int x, y;

public:
	SpaceShip(SDL_Renderer*, int, int);
	void display();
	void change_x(bool);
	void change_y(bool);

};

#endif