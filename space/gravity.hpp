#ifndef SPACE_GRAVITY_HPP_
#define SPACE_GRAVITY_HPP_

#include <list>
#include "space/ship.hpp"
#include "space_object.hpp"

class SpaceObject;

namespace space {

const float gravitationalConstant {0.000000000066742};

// DENSITIES
const float asteroidDensity {1.0};
const float shipDensity {0.5};

class Gravity {
private:
    primitive::Direction inertia {0.0, 0.0};
    double default_offset {1.0};
    double mass;
public:
    Gravity(double mass) : mass{mass}{};
    primitive::Direction get_offset(std::list<SpaceObject*> *stars, primitive::Point);
    void update_inertia(primitive::Direction);
};

} // namespace space

#endif