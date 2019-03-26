#include "explosion.hpp"


Explosion::Fragment::Fragment(Point p, Point next_p, int dots_number=3){

    this->initial_p = p;
    this->dots_number = dots_number;
    this->x_shift = p.x - next_p.x;
    this->y_shift = p.y - next_p.y;
    this->angle = 0.1 + rand() % 15;

    int distribution_value = 25;
    for (int i = 0; i < this->dots_number; ++i){
        dots.push_back(get_rotated_point(initial_p, Point(initial_p.x + rand() % distribution_value, initial_p.y)));
    }
    fragment_shift_delay = NOW + static_cast<std::chrono::milliseconds> (FRAGMENT_SHIFT_DELAY);
}

Explosion::Fragment::Fragment(Point p1, Point p2, Point p3){

    double tmp_x = p1.x/3 + p2.x/3 + p3.x/3;
    double tmp_y = p1.y/3 + p2.y/3 + p3.y/3;

    this->x_shift = abs(tmp_x - p1.x)/3 + abs(tmp_x - p2.x)/3 + abs(tmp_x - p3.x)/3;
    this->y_shift = abs(tmp_y - p1.y)/3 + abs(tmp_y - p2.y)/3 + abs(tmp_y - p3.y)/3;

    this->angle = 0.1 + rand() % 15;

    dots.push_back(p1);
    dots.push_back(p2);
    dots.push_back(p3);
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

    double center_x = 0.0;
    double center_y = 0.0;

    int dots_number = dots.size();
    for(auto iter = dots.begin(); iter != dots.end(); ++iter){
        center_x += iter->x / dots_number;
        center_y += iter->y / dots_number;
        iter->x += this->x_shift;
        iter->y += this->y_shift;
    }

    for(auto iter = dots.begin(); iter != dots.end(); ++iter){
        *iter = get_rotated_point(*iter, Point(center_x, center_y), this->angle);
    }

    fragment_shift_delay = NOW + (std::chrono::milliseconds) FRAGMENT_SHIFT_DELAY;
}

Explosion::Explosion(Point p, SDL_Renderer *renderer, Asteroid *ast){

    destroy_time = NOW + static_cast<std::chrono::milliseconds> (EXPLOSION_LIFE_TIME);
    this->p = p;
    this->renderer = renderer;
    Point p_center = ast->getCenterPoint();

    int random_number_of_ragments = 5 + rand() % 10;
    double angle_diff = 360 / random_number_of_ragments;
    auto p_first = ast->getPoints().begin();
    auto p_second = p_first + 1;

    while (p_second != ast->getPoints().end()){
        fragments.push_back(Fragment(**p_first, **p_second, p_center));
        ++p_first;
        ++p_second;
    }
    p_second = ast->getPoints().begin();
    fragments.push_back(Fragment(**p_first, **p_second, p_center));
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
