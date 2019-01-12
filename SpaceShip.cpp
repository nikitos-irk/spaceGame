#include "SpaceShip.hpp" 

SpaceShip::SpaceShip(SDL_Renderer *renderer, int x, int y){
	this->renderer = renderer;
	this->x = x;
	this->y = y;
}

void SpaceShip::change_y(bool forward){
	
	int mediana_x = points[1].x/2 + points[2].x/2;
	int mediana_y = points[1].y/2 + points[2].y/2;

	int diff_x = (mediana_x - points[0].x)/5;
	int diff_y = (mediana_y - points[0].y)/5;

	if (!forward){
		diff_x *= -1;
		diff_y *= -1;
	}

	for (int i = 0; i < POINTS_COUNT; ++i){
		points[i].x += diff_x;
		points[i].y += diff_y;
	} 
}
void SpaceShip::change_x(bool clockwise){
	
	float angel = M_PI / 6;
	if (!clockwise) { angel *= -1; }
	
	int relative_x = 0;
	int relative_y = 0;

	for (int i = 0; i < POINTS_COUNT; ++i){	
		relative_x += points[i].x;
		relative_y += points[i].y;
	}

	relative_x = relative_x/POINTS_COUNT;
	relative_y = relative_y/POINTS_COUNT;
	
	int tmp_x = 0;
	int tmp_y = 0;

	for (int i = 0; i < POINTS_COUNT; ++i){
		tmp_x = (points[i].x - relative_x) * cos(angel) - (points[i].y - relative_y) * sin(angel) + relative_x;
		tmp_y = (points[i].x - relative_x) * sin(angel) + (points[i].y - relative_y) * cos(angel) + relative_y;
		points[i].x = tmp_x;
		points[i].y = tmp_y;
		
	}
}

void SpaceShip::display(){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLines(renderer, points, POINTS_COUNT);
}