#ifndef COMMON_HPP
#define COMMON_HPP

#include <complex>
#include <cmath>
#include <cstdlib>

struct SDL_Renderer;
class ColorGenerator;

#define NOW std::chrono::system_clock::now()
typedef std::complex<double> point;

struct Point{
private:
    bool doubleEquals(double a, double b, double epsilon = 0.000000001)
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
        return doubleEquals(x, p.x) && doubleEquals(y, p.y);
    }
    bool operator!=(const Point& p){
        return doubleEquals(x, p.x) || doubleEquals(y, p.y);
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
    DirectionXY(const DirectionXY& tmp_xy) : x(tmp_xy.x), y(tmp_xy.y){}
};

struct DirectionalVector{
    DirectionXY p1, p2;
    DirectionalVector();
    DirectionalVector(DirectionXY, DirectionXY);
};


Point get_rotated_point(Point, Point, double angle=0.0);

inline double getLengthOfVector(Point px1, Point px2) { return sqrt(pow(px1.x - px2.x, 2) + pow(px1.y - px2.y, 2)); }
std::pair<double, double> getXYOffsetOnVector(Point, Point, double);
Point getTwoLinesIntersaction(Point, Point, Point, Point);
void putSquareOnPoint(SDL_Renderer*, Point, double, double);
void fillRect(SDL_Renderer*, Point, Point, Point);
std::pair<Point, Point> getPerpendicularLineByPoint(Point, Point, Point, double);
void updateSkeleton(ColorGenerator*, SDL_Renderer*, double, double, Point, Point, Point, double, bool, bool);

class Common {
public:
    Common();
};

#endif // COMMON_HPP
