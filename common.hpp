#ifndef COMMON_HPP
#define COMMON_HPP
#include <mutex>
#include <utility>
#include <cstdlib>
#include <complex>
#include <utility>
#include <chrono>
#include <vector>
#include <functional>
#include <unistd.h>
#include "SDL2/SDL.h"
#include "colorschema.hpp"

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

struct DirectionXY{
    double x;
    double y;
    DirectionXY(double, double);
    DirectionXY();

    DirectionXY& operator *=(int value){
        x *= value;
        y *= value;
        return *this;
    }
    DirectionXY(const DirectionXY& tmpXY) : x(tmpXY.x), y(tmpXY.y){}
};

struct DirectionalVector{
    DirectionXY p1, p2;
    DirectionalVector();
    DirectionalVector(DirectionXY, DirectionXY);
};


Point get_rotated_point(Point, Point, double angle=0.0);

inline double getLengthOfVector(Point px1, Point px2) { return sqrt(pow(px1.x - px2.x, 2) + pow(px1.y - px2.y, 2)); }
pair<double, double> getXYOffsetOnVector(Point, Point, double);
Point getTwoLinesIntersaction(Point, Point, Point, Point);
void putSquareOnPoint(SDL_Renderer*, Point, double, double);
void fillRect(SDL_Renderer*, Point, Point, Point);
pair<Point, Point> getPerpendicularLineByPoint(Point, Point, Point, double);
void updateSkeleton(colorGenerator*, SDL_Renderer*, double, double, Point, Point, Point, double, bool, bool);

class Common {
public:
    Common();
};

#endif // COMMON_HPP
