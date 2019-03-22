#include "explosion.hpp"

Explosion::Fragment::Fragment(Point p, Point next_p, int dots_number=3){

    this->initial_p = p;
    this->dots_number = dots_number;

    int distribution_value = 25;
    for (int i = 0; i < this->dots_number; ++i){
        dots.push_back(get_rotated_point(initial_p,
                                         Point(initial_p.x + rand() % distribution_value,
                                               initial_p.y + rand() % distribution_value)));
    }
}

void Explosion::Fragment::show(SDL_Renderer *renderer){
    int x1, y1, x2, y2;
    for(auto iter = dots.begin(); iter != dots.end() - 1; ++iter){
        x1 = iter->x;
        y1 = iter->y;
        x2 = (iter + 1)->x;
        y2 = (iter + 1)->y;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
    x2 = dots.begin()->x;
    y2 = dots.begin()->y;
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

Explosion::Explosion(Point p, SDL_Renderer *renderer){
    this->p = p;
    this->renderer = renderer;

    int random_number_of_ragments = 5 + rand() % 10;
    double angle_diff = 360 / random_number_of_ragments;

    for (int i = 0; i < random_number_of_ragments; ++i){

        Point tmp = get_rotated_point(p, Point(p.x + FRAGMENT_SHIFT, p.y + FRAGMENT_SHIFT), angle_diff * i);
        fragments.push_back(Fragment(p, tmp, 3 + rand() % 3));
    }
}

void Explosion::bang(){

}
