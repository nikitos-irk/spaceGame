#ifndef COMMON_HPP
#define COMMON_HPP
#include <utility>
#include <cstdlib>
#include <complex>
#include <utility>

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

Point get_rotated_point(Point, Point, double angle=0.0);
class Common {
public:
    Common();
};

#endif // COMMON_HPP
