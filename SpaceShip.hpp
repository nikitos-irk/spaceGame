#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "SDL2/SDL.h"
#include <utility>
#include <functional>
#include <vector>
#include <iostream>
#include <chrono>
#include <time.h>
#include "speed.hpp"

#define BLOCK_SIZE 5
#define POINTS_COUNT 4
#define ASTEROID_POINTS_COUNT 7
#define ROTATION_DELAY 60
#define DISPLAY_DELAY 100
#define SHOOTING_DELAY 500
#define CHANGE_POSITION_DELAY 100
#define INERTIA_DELAY 10
#define INERTIA_COUNTER 500

using namespace std;

class SpaceObject{
protected:
	SDL_Renderer *renderer;
	int x, y;
	int screen_width;
	int screen_height;
	std::chrono::time_point<std::chrono::system_clock> display_delay; 
	std::chrono::time_point<std::chrono::system_clock> rotation_delay;
public:
	SpaceObject(SDL_Renderer*, int, int, int, int);
	virtual void display() = 0;
	virtual void change_position(DirectionXY);
    virtual ~SpaceObject();
};

class Projectile: public SpaceObject{
private:
	int direction_x;
	int direction_y;
public:
    Projectile(SDL_Renderer*, int, int, int, int, int, int);
	void change_position(DirectionXY);
    void display();
    ~Projectile();
};

class SpaceShip: public SpaceObject{
private:
	SDL_Point points[POINTS_COUNT];
	std::chrono::time_point<std::chrono::system_clock> shoot_delay; 
public:
    Speed *speed;
    void slowdown();
    void accelarate();
    void backward_slowdown();
    void backward_accelarate();
    SpaceShip(SDL_Renderer*, int, int, int);
	Projectile * shoot();
	vector<Projectile*> projectiles;
	DirectionXY get_direction();
    DirectionXY get_offset();
	void display();
	void change_x(bool);
	void change_y(bool);
    DirectionalVector getDerectionalVector();
    ~SpaceShip();
};

class Asteroid: public SpaceObject{
public:
	void display();
	Asteroid(SDL_Renderer*, int, int, int, int);
	SDL_Point points[ASTEROID_POINTS_COUNT];
	void change_position(DirectionXY);
    ~Asteroid();
};

#endif
