#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>
#include <cstdlib>

#include "primitive/point.hpp"

struct SDL_Renderer;
class ColorGenerator;

inline double getLengthOfVector(primitive::Point px1, primitive::Point px2)
{ return sqrt(pow(px1.x - px2.x, 2) + pow(px1.y - px2.y, 2)); }
std::pair<double, double> getXYOffsetOnVector(primitive::Point, primitive::Point, double);
primitive::Point getTwoLinesIntersaction(primitive::Point, primitive::Point,
                                         primitive::Point, primitive::Point);
void putSquareOnPoint(SDL_Renderer*, primitive::Point, double, double);
void fillRect(SDL_Renderer*, primitive::Point, primitive::Point, primitive::Point);
std::pair<primitive::Point, primitive::Point> getPerpendicularLineByPoint(
    primitive::Point, primitive::Point, primitive::Point, double);
void updateSkeleton(ColorGenerator*, SDL_Renderer*, double, double, primitive::Point,
                    primitive::Point, primitive::Point, double, bool, bool);

#endif // COMMON_HPP
