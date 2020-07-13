#ifndef SPACE_GRAVITY_HPP_
#define SPACE_GRAVITY_HPP_

namespace space{

const float gravitationalConstant {6.67430};

// DENSITIES
const float asteroidDensity {1.0};
const float shipDensity {0.5};

class Gravity {
public:
    float mass;
    Gravity(float mass): mass{mass}{};
    Gravity() = default;
};

}

#endif