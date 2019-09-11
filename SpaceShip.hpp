#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>

#include <SDL2/SDL.h>

#include "colorschema.hpp"
#include "speed.hpp"

constexpr auto BLOCK_SIZE = 5;
constexpr auto POINTS_COUNT = 4;
constexpr auto ASTEROID_POINTS_COUNT = 7;
constexpr auto ROTATION_DELAY = 60;
constexpr auto DISPLAY_DELAY = 10;
constexpr auto SHOOTING_DELAY = 100;
constexpr auto CHANGE_POSITION_DELAY = 30;
constexpr auto INERTIA_DELAY = 10;
constexpr auto INERTIA_COUNTER = 500;
constexpr auto PROJ_LIFETIME = 5000;
constexpr auto SHIP_COLOR_CHANGE = 50;

struct Nozzle{
    std::vector <Point> points;
    std::vector <Point> originPoints;
    SDL_Renderer* renderer;
    ColorSchema *cs;
    Speed *speed;

    Nozzle(SDL_Renderer* renderer, Point a, Point b, Point c, Speed *speed){
        originPoints.push_back(a);
        originPoints.push_back(b);
        originPoints.push_back(c);
        copy(originPoints.begin(), originPoints.end(), back_inserter(points));
        this->renderer = renderer;
        this->speed = speed;
        cs = new ColorSchema(Color(255, 17, 0), Color(255, 237, 0));
    }
    void display(){
        cs->update(speed->getCurrentA());
        SDL_SetRenderDrawColor(renderer, cs->getR(), cs->getG(), cs->getB(), 255);
        SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[1].x, points[1].y);
        SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[2].x, points[2].y);
        SDL_RenderDrawLine(renderer, points[1].x, points[1].y, points[2].x, points[2].y);
    }


    void update(){
        double offsetLength = speed->getCurrentA();
        points.clear();
        copy(originPoints.begin(), originPoints.end(), back_inserter(points));

        if (offsetLength < 0.001) { return; }

        double Cx = (points[0].x - points[1].x) * (offsetLength);
        double Cy = (points[0].y - points[1].y) * (offsetLength);

        double Cx2 = (points[1].x - points[2].x) * (offsetLength);
        double Cy2 = (points[1].y - points[2].y) * (offsetLength);

        points[0].x += Cx;
        points[0].y += Cy;

        points[2].x -= Cx2;
        points[2].y -= Cy2;
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
    bool alive;
public:
	SpaceObject(SDL_Renderer*, int, int, int, int);
	virtual void display(bool) = 0;
	virtual void change_position(DirectionXY);
    virtual ~SpaceObject();
    bool isAlive();
    void markAsDead();
};

class Projectile: public SpaceObject{
private:
	int direction_x;
	int direction_y;
    double x_previous;
    double y_previous;
    std::chrono::time_point<std::chrono::system_clock> life_time;
public:
    Projectile(SDL_Renderer*, int, int, int, int, int, int);
	void change_position(DirectionXY);
    void display(bool);
    Point* getXY();
    std::pair<Point, Point> getLine();
    std::chrono::time_point<std::chrono::system_clock> getLifeTime();
    ~Projectile();
};

class SpaceShip: public SpaceObject{
private:
    ColorSchema *cs;
    std::vector<Point> skeleton;
    std::chrono::time_point<std::chrono::system_clock> shoot_delay;
    std::chrono::time_point<std::chrono::system_clock> ship_color_change;
    Point initialMedianIntersection;
    double getTiltAngel();
    double getLengthOfBase();
    int spaceWidth;
    int spaceHeight;
    double nozzleMinHeight;
    double nozzleMaxHeight;
    int nozzleWidth;
    Nozzle *leftNozzle;
    Nozzle *rightNozzle;
public:
    colorGeneratorShip *cg;
    std::vector<Point> pp;
    void updateNozzles();
    double getCurrentA();
    Point getMedianIntersaction();
    Speed *speed;
    void slowdown();
    void accelarate();
    void backward_slowdown();
    void backward_accelarate();
    SpaceShip(SDL_Renderer*, int, int, int);
	Projectile * shoot();
  std::vector<Projectile*> projectiles;
	DirectionXY get_direction();
    DirectionXY get_offset();
	void display(bool);
	void change_x(bool);
	void change_y(bool);
    DirectionalVector getDerectionalVector();
    ~SpaceShip();
};

class Asteroid: public SpaceObject{
private:
    std::vector<Point*> pp;
    void fill();
public:
    colorGeneratorAsteroid *cg;
    Point getCenterPoint();
	void display(bool);
  std::vector<Point*>& getPoints();
	Asteroid(SDL_Renderer*, int, int, int, int);
	void change_position(DirectionXY);
    Point* getFirstPoint();
    ~Asteroid();
};

#endif
