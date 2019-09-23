#include "point.hpp"

#include <complex>

namespace primitive {

Point& Point::rotate(Point const& center, double angle)
{
    std::complex<double> P{x, y};
    std::complex<double> Q(center.x, center.y);
    std::complex<double> P_rotated = (P-Q) * std::polar(1.0, angle) + Q;
    x = P_rotated.real();
    y = P_rotated.imag();
    return *this;
}

Point& Point::move(Direction const& d)
{
  x += d.x;
  y += d.y;
  return *this;
}

Direction& Direction::operator *=(int value)
{
    x *= value;
    y *= value;
    return *this;
}

}  // namespace primitive
