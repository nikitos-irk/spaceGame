#include "SDL2/SDL.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H

const int COEF = 5;
const int CITIZEN_SIZE = 5 * COEF;
const int CITIZEN_STEP = 5 * COEF;
// extern const int SCREEN_WIDTH = 640;
// extern const int SCREEN_HEIGHT = 480;
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Background{
private:
	SDL_Renderer *renderer;
public:
	Background(SDL_Renderer *renderer);
	void fill_background();
	void draw_grid();
};

#endif