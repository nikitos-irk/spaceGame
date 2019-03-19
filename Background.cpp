#include "Background.hpp"


Background::Background(SDL_Renderer *renderer, int screen_width, int screen_height){
	this->renderer = renderer;
	this->screen_width = screen_width;
	this->screen_height = screen_height;
}

void Background::fill_background(){
	int gradient_size = 1;
	int steps = 1 + (screen_width / (CITIZEN_SIZE * gradient_size));

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = screen_width;
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
	
	while (rect.y < screen_height) {
		r = (r_start < r_finish) ? r + r_step : r - r_step;
		g = (g_start < g_finish) ? g + g_step : g - g_step;
		b = (b_start < b_finish) ? b + b_step : b - b_step;

		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderFillRect(renderer, &rect);
		rect.x = 0;
		rect.y += CITIZEN_SIZE * gradient_size;
	}
}

void Background::draw_grid(){
	
	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	
	for (int i = 0; i < screen_height; i = i + CITIZEN_SIZE)
		SDL_RenderDrawLine(renderer, 0, i, screen_width, i);

	for (int i = 0; i < screen_width; i = i + CITIZEN_SIZE)
		SDL_RenderDrawLine(renderer, i, 0, i, screen_height);
}
