#include "gravity.hpp"
#include <list>
#include <iostream>


namespace space {


primitive::Direction Gravity::get_offset(std::list<SpaceObject*> *stars, primitive::Point object_center) {
    primitive::Direction result {0.0, 0.0};
    
    for (auto star = stars->begin(); star != stars->end(); ++star){
        
        auto star_mass {dynamic_cast<Star*>(*star)->getMass()};
        auto star_center {dynamic_cast<Star*>(*star)->getCoordinates()};

        auto distance = primitive::Line{star_center, object_center}.length();
        auto F = gravitationalConstant * (mass * star_mass) / std::pow(distance, 2);

        auto step = distance / default_offset;

        auto offset_x = F * abs(abs(star_center.x) - abs(object_center.x)) / step;
        auto offset_y = F * abs(abs(star_center.y) - abs(object_center.y)) / step;
        
        if (object_center.x > star_center.x) { offset_x = -offset_x; }
        if (object_center.y > star_center.y) { offset_y = -offset_y; }

        result.x += offset_x;
        result.y += offset_y;
    }
    
    inertia.x += result.x;
    inertia.y += result.y;
    
    return inertia;
}

void Gravity::update_inertia(primitive::Direction offset){
    std::cout << "inertia.x = " << inertia.x << std::endl;
    std::cout << "offset.x = " << offset.x << std::endl;
    if (inertia.x * offset.x > 0 && inertia.x - offset.x < 0.0001) {
        inertia.x = 0;
    } else if (inertia.x * offset.x > 0 and offset.x > 0.0001) {
        inertia.x -= offset.x;
    }

    if (inertia.y * offset.y > 0 && inertia.y - offset.y < 0.0001) {
        inertia.y = 0;
    } else if (inertia.y * offset.y > 0 and offset.y > 0.0001) {
        inertia.y -= offset.y;
    }
    // inertia.x -= offset.x;
    // inertia.y -= offset.y;
}

} // namespace space
