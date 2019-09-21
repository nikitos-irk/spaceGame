#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>
#include <cstdlib>

#include "primitive/point.hpp"
#include "primitive/line.hpp"

struct SDL_Renderer;
class ColorGenerator;

void updateSkeleton(ColorGenerator*, SDL_Renderer*, double, double, primitive::Point,
                    primitive::Point, primitive::Point, double, bool, bool);

#endif // COMMON_HPP
