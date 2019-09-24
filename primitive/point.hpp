#ifndef PRIMITIVE_POINT_HPP_
#define PRIMITIVE_POINT_HPP_

#include <cmath>

namespace {
bool doubleEquals(double a, double b, double epsilon = 0.000000001)
{
    return std::abs(a - b) < epsilon;
}
}  // namespace

namespace primitive
{
struct Direction {
    double x{0.0};
    double y{0.0};
    Direction & operator *=(int value);
};

struct Point{
    double x{0.0};
    double y{0.0};
    Point& rotate(Point const& center, double angle);
    Point& move(Direction const& d);
};

inline bool operator==(const Point& p1, const Point& p2) {
    return doubleEquals(p1.x, p2.x) && doubleEquals(p1.y, p2.y);
}
inline bool operator!=(const Point& p1, const Point& p2) {
    return !(p1 == p2);
}

inline Point median(Point const& p1, Point const& p2)
{
  return {p1.x/2 + p2.x/2, p1.y/2 + p2.y/2};
}

}  // namespace primitive

#endif /* PRIMITIVE_POINT_HPP_ */
