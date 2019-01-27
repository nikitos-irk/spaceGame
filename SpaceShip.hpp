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
#include <complex>

#define BLOCK_SIZE 5
#define POINTS_COUNT 4
#define ASTEROID_POINTS_COUNT 7
#define ROTATION_DELAY 60
#define DISPLAY_DELAY 50
#define SHOOTING_DELAY 100
#define CHANGE_POSITION_DELAY 100
#define INERTIA_DELAY 10
#define INERTIA_COUNTER 500

using namespace std;
typedef complex<double> point;

struct Point{
private:
    bool double_equals(double a, double b, double epsilon = 0.000000001)
    {
        return std::abs(a - b) < epsilon;
    }
public:
    double x;
    double y;
    Point(double x, double y){
        this->x = x;
        this->y = y;
    }
    Point(){
        this->x = 0.0;
        this->y = 0.0;
    }
    bool operator==(const Point& p){
        return double_equals(x, p.x) && double_equals(y, p.y);
    }
    bool operator!=(const Point& p){
        return double_equals(x, p.x) || double_equals(y, p.y);
    }
};

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
    vector<Point> pp;
    std::chrono::time_point<std::chrono::system_clock> shoot_delay;
    double px;
    double py;
    Point initialMedianIntersection;
public:
    Point getMedianIntersaction();
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
private:
    vector<Point*> pp;
public:
	void display();
	Asteroid(SDL_Renderer*, int, int, int, int);
	void change_position(DirectionXY);
    Point* getFirstPoint();
    ~Asteroid();
};

#endif
