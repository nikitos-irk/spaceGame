#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "SDL2/SDL.h"
#include <utility>
#include <functional>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>
#include <time.h>

#define BLOCK_SIZE 5
#define POINTS_COUNT 4
#define ASTEROID_POINTS_COUNT 7
#define ROTATION_DELAY 60
#define DISPLAY_DELAY 100
#define SHOOTING_DELAY 30
#define CHANGE_POSITION_DELAY 10
#define INERTIA_DELAY 10
#define INERTIA_COUNTER 500

extern const int SCREEN_WIDTH = 640;
extern const int SCREEN_HEIGHT = 480;

using namespace std;

static SDL_Point points[POINTS_COUNT] = {
    {SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
    {SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 + SCREEN_HEIGHT / 10},
    {SCREEN_WIDTH/2 + 10, SCREEN_HEIGHT/2 + SCREEN_HEIGHT / 10},
    {SCREEN_WIDTH/2, SCREEN_HEIGHT/2}
};

struct DirectionXY{
	int x;
	int y;
	DirectionXY(int, int);
};

class SpaceObject{
protected:
	SDL_Renderer *renderer;
	int x, y;
	std::chrono::time_point<std::chrono::system_clock> display_delay; 
	std::chrono::time_point<std::chrono::system_clock> rotation_delay;
public:
	SpaceObject(SDL_Renderer*, int, int);
	virtual void display();
	virtual void change_position(DirectionXY);
	void change_x(bool);
	void change_y(bool);	
};

class Projectile: public SpaceObject{
private:
	int direction_x;
	int direction_y;
public:
	Projectile(SDL_Renderer*, int, int, int, int);
	void change_position(DirectionXY);
	void display();
};

class SpaceShip: public SpaceObject{
private:
	std::chrono::time_point<std::chrono::system_clock> shoot_delay; 
public:
	SpaceShip(SDL_Renderer*, int, int);
	Projectile * shoot();
	vector<Projectile*> projectiles;
	DirectionXY get_direction();
};

class Asteroid: public SpaceObject{
public:
	void display();
	Asteroid(SDL_Renderer*, int, int, int, int);
	SDL_Point points[ASTEROID_POINTS_COUNT];
	void change_position(DirectionXY);
};

#endif