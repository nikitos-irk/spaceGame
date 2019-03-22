#include "explosion.hpp"


Fragment::Fragment(Point p, int dots_number=3){

    this->initial_p = p;
    this->dots_number = dots_number;

    int distribution_value = 25;
    for (int i = 0; i < this->dots_number; ++i){
        dots.push_back(get_rotated_point(initial_p,
                                         Point(initial_p.x + rand() % distribution_value,
                                               initial_p.y + rand() % distribution_value)));
    }
}

Explosion::Explosion(Point p){
    this->p = p;
}

void Explosion::bang(){

}
