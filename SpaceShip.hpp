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
#include <utility>
#include "colorschema.hpp"

#define BLOCK_SIZE 5
#define POINTS_COUNT 4
#define ASTEROID_POINTS_COUNT 7
#define ROTATION_DELAY 60
#define DISPLAY_DELAY 10
#define SHOOTING_DELAY 100
#define CHANGE_POSITION_DELAY 50
#define INERTIA_DELAY 10
#define INERTIA_COUNTER 500
#define PROJ_LIFETIME 5000
#define SHIP_COLOR_CHANGE 50
#define NOW std::chrono::system_clock::now()

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

struct Nozzle{
    vector <Point> points;
    vector <Point> originPoints;
    SDL_Renderer* renderer;
    ColorSchema *csForward;
    ColorSchema *csBackward;
    Speed *speed;

    Nozzle(SDL_Renderer* renderer, Point a, Point b, Point c, Speed *speed){
        originPoints.push_back(a);
        originPoints.push_back(b);
        originPoints.push_back(c);
        copy(originPoints.begin(), originPoints.end(), back_inserter(points));
        this->renderer = renderer;
        this->speed = speed;
        csForward  = new ColorSchema(Color(255, 17, 0), Color(255, 237, 0));
        csBackward = new ColorSchema(Color(255, 17, 0), Color(100,149,237));
    }
    void display(){
        cout << "speed->getCurrentA() = " << speed->getCurrentA() << endl;
        cout << "abs(speed->getForwardA()) < abs(speed->getBackwardA()) = " << (abs(speed->getForwardA())) << " : " <<  (abs(speed->getBackwardA())) << endl;
        if (abs(speed->getForwardA()) < abs(speed->getBackwardA())){
            csForward->update(speed->getCurrentA());
            SDL_SetRenderDrawColor(renderer, csForward->getR(),  csForward->getG(),  csForward->getB(), 255);
        } else {
            csBackward->update(speed->getCurrentA());
            SDL_SetRenderDrawColor(renderer, csBackward->getR(), csBackward->getG(), csBackward->getB(), 255);
        }
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
	virtual void display() = 0;
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
    void display();
    Point* getXY();
    pair<Point, Point> getLine();
    std::chrono::time_point<std::chrono::system_clock> getLifeTime();
    ~Projectile();
};

class SpaceShip: public SpaceObject{
private:
    ColorSchema *cs;
    vector<Point> pp;
    vector<Point> skeleton;
    std::chrono::time_point<std::chrono::system_clock> shoot_delay;
    std::chrono::time_point<std::chrono::system_clock> ship_color_change;
    Point initialMedianIntersection;
    double getTiltAngel();
    void putSquareOnPoint(Point, double);
    void updateSkeleton(Point, Point, Point, double, bool, bool);
    pair<Point, Point> getPerpendicularLineByPoint(Point, Point, Point);
    double getLengthOfBase();
    Point getTwoLinesIntersaction(Point, Point, Point, Point);
    pair<double, double> getXYOffsetOnVector(Point, Point, double);
    void fillRect(Point, Point, Point);
    int spaceWidth;
    int spaceHeight;
    double nozzleMinHeight;
    double nozzleMaxHeight;
    int nozzleWidth;
    Nozzle *leftNozzle;
    Nozzle *rightNozzle;
    vector<Color> availableColors;
    vector<Color>::iterator colorIter;
public:
    Color getRandomColor();
    Color getNextColor();
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
    vector<Point*>& getPoints();
	Asteroid(SDL_Renderer*, int, int, int, int);
	void change_position(DirectionXY);
    Point* getFirstPoint();
    ~Asteroid();
};

#endif
