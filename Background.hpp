#include "SDL2/SDL.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H

const int COEF = 5;
const int CITIZEN_SIZE = 5 * COEF;
const int CITIZEN_STEP = 5 * COEF;

class Background{
private:
	int screen_width;
	int screen_height;
	SDL_Renderer *renderer;
public:
	Background(SDL_Renderer*, int, int);
	void fill_background();
	void draw_grid();
};

#endif