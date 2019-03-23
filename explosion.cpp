#include "explosion.hpp"

Explosion::Fragment::Fragment(Point p, Point next_p, int dots_number=3){

    this->initial_p = p;
    this->dots_number = dots_number;
    this->x_shift = p.x - next_p.x;
    this->y_shift = p.y - next_p.y;

    int distribution_value = 25;
    for (int i = 0; i < this->dots_number; ++i){
        dots.push_back(get_rotated_point(initial_p, Point(initial_p.x + rand() % distribution_value, initial_p.y)));
    }
    fragment_shift_delay = NOW + static_cast<std::chrono::milliseconds> (FRAGMENT_SHIFT_DELAY);
}

void Explosion::Fragment::display(SDL_Renderer *renderer){

    int x1, y1, x2, y2;

    auto iter = dots.begin();
    auto iter_next = iter + 1;

    while (iter_next != dots.end()){
        x1 = iter->x;
        y1 = iter->y;
        x2 = iter_next->x;
        y2 = iter_next->y;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        ++iter; ++iter_next;
    }
    iter = dots.begin();
    x1 = iter->x;
    y1 = iter->y;
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    shift();
}

void Explosion::Fragment::shift(){

    if (fragment_shift_delay > NOW) { return; }

    for(auto iter = dots.begin(); iter != dots.end(); ++iter){
        iter->x += this->x_shift;
        iter->y += this->y_shift;
    }

    fragment_shift_delay = NOW + (std::chrono::milliseconds) FRAGMENT_SHIFT_DELAY;
}

Explosion::Explosion(Point p, SDL_Renderer *renderer){

    destroy_time = NOW + static_cast<std::chrono::milliseconds> (EXPLOSION_LIFE_TIME);
    this->p = p;
    this->renderer = renderer;

    int random_number_of_ragments = 5 + rand() % 10;
    double angle_diff = 360 / random_number_of_ragments;

    for (int i = 0; i < random_number_of_ragments; ++i){
        int random_x = 5 + rand() % 5;
        Point tmp = get_rotated_point(p, Point(p.x + random_x, p.y), angle_diff * i);
        fragments.push_back(Fragment(p, tmp, 3 + rand() % 3));
    }
}

void Explosion::display(){
    for (auto iter = fragments.begin(); iter != fragments.end(); ++iter){
        iter->display(renderer);
    }
}

bool Explosion::isAlive(){ return NOW < destroy_time; }

void Explosion::shift(DirectionXY shiftValue){

    for (auto fragment = fragments.begin(); fragment != fragments.end(); ++fragment){
        for(auto iter = fragment->dots.begin(); iter != fragment->dots.end(); ++iter){
            iter->x += shiftValue.x;
            iter->y += shiftValue.y;
        }
    }

}
