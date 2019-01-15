#include "SpaceShip.hpp"

SpaceObject::SpaceObject(SDL_Renderer *renderer, int x, int y){
	this->renderer = renderer;
	this->x = x;
	this->y = y;
	time_delay = std::chrono::system_clock::now() + (std::chrono::milliseconds) 100;
}

Projectile::Projectile(SDL_Renderer *renderer, int x, int y, int direction_x, int direction_y): SpaceObject::SpaceObject(renderer, x, y){
	this->direction_x = direction_x;
	this->direction_y = direction_y;
}

SpaceShip::SpaceShip(SDL_Renderer *renderer, int x, int y): SpaceObject::SpaceObject(renderer, x, y){}

DirectionXY::DirectionXY(int x, int y){
	this->x = x;
	this->y = y;
}

DirectionXY SpaceShip::get_direction(){
	int mediana_x = points[1].x/2 + points[2].x/2;
	int mediana_y = points[1].y/2 + points[2].y/2;
	int diff_x = (mediana_x - points[0].x)/5;
	int diff_y = (mediana_y - points[0].y)/5;
	return DirectionXY(diff_x, diff_y);
}

void SpaceObject::change_position(DirectionXY directionXY){
	this->x += directionXY.x;
	this->y += directionXY.y;
}

void SpaceObject::change_y(bool forward){
	
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
void SpaceObject::change_x(bool clockwise){
	
	float angel = M_PI / 12;
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

void SpaceObject::display(){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLines(renderer, points, POINTS_COUNT);
}

Projectile * SpaceShip::shoot(){
	int mediana_x = points[1].x/2 + points[2].x/2;
	int mediana_y = points[1].y/2 + points[2].y/2;
	int diff_x = (mediana_x - points[0].x)/5;
	int diff_y = (mediana_y - points[0].y)/5;
	
	Projectile *projectile = new Projectile(this->renderer, points[0].x - diff_x, points[0].y - diff_y, diff_x, diff_y);
	// projectiles.push_back(projectile);
	return projectile;
}

void Projectile::display(){

	double error = (double) - BLOCK_SIZE;
	double tmp_x = (double) BLOCK_SIZE - 0.5;
	double tmp_y = (double)0.5;
	
	double cx = this->x - 0.5;
	double cy = this->y - 0.5;
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	
	while (tmp_x >= tmp_y){
		SDL_RenderDrawPoint(renderer, (int)(cx + tmp_x), (int)(cy + tmp_y));
		SDL_RenderDrawPoint(renderer, (int)(cx + tmp_y), (int)(cy + tmp_x));

		if (tmp_x != 0){
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_x), (int)(cy + tmp_y));
			SDL_RenderDrawPoint(renderer, (int)(cx + tmp_y), (int)(cy - tmp_x));
		}

		if (tmp_y != 0){
			SDL_RenderDrawPoint(renderer, (int)(cx + tmp_x), (int)(cy - tmp_y));
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_y), (int)(cy + tmp_x));
		}

		if (tmp_x != 0 && tmp_y != 0){
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_x), (int)(cy - tmp_y));
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_y), (int)(cy - tmp_x));
		}

		error += tmp_y;
		++tmp_y;
		error += tmp_y;

		if (error >= 0){
			--tmp_x;
			error -= tmp_x;
			error -= tmp_x;
		}
	}
	for (double dy = 1; dy <= BLOCK_SIZE; dy += 1.0){
		double dx = floor(sqrt((2.0 * BLOCK_SIZE * dy) - (dy * dy)));
		double cx = this->x;
		double cy = this->y;
		int x = cx - dx;
		SDL_RenderDrawLine(renderer, cx - dx, cy + dy - BLOCK_SIZE, cx + dx, cy + dy - BLOCK_SIZE);
		SDL_RenderDrawLine(renderer, cx - dx, cy - dy + BLOCK_SIZE, cx + dx, cy - dy + BLOCK_SIZE);
	}
	if (this->time_delay < std::chrono::system_clock::now()){
		this->x -= direction_x;
		this->y -= direction_y;
		time_delay = std::chrono::system_clock::now() + (std::chrono::milliseconds) 100;
	}
}

Asteroid::Asteroid(SDL_Renderer *renderer, int x, int y, int direction_x, int direction_y): SpaceObject::SpaceObject(renderer, x, y){
	//TODO: think about direction_x;direction_y

	int error_x = rand() % 10;
	int error_y = rand() % 10;
    
	points[0] = {x + error_x, 			y + error_y};
	points[1] = {x - 10 + rand() % 5, 	y + 10 + rand() % 5};
	points[2] = {x + rand() % 5, 		y + 20 + rand() % 5};
	points[3] = {x + 10 + rand() % 5, 	y + 20 + rand() % 5};
	points[4] = {x + 20 + rand() % 5, 	y + 10 + rand() % 5};
	points[5] = {x + 10 + rand() % 5, 	y + rand() % 5};
	points[6] = {x + error_x, 			y + error_y};
}

void Asteroid::display(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawLines(renderer, points, ASTEROID_POINTS_COUNT);
}

void Asteroid::change_position(DirectionXY directionXY){
	this->x += directionXY.x;
	this->y += directionXY.y;
	for (int i = 0; i < ASTEROID_POINTS_COUNT; ++i){
		this->points[i].x += directionXY.x;
		this->points[i].y += directionXY.y;
	}
}