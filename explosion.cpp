#include "explosion.hpp"

#include "space_ship.hpp"
#include "primitive/line.hpp"
#include "skeleton.hpp"

constexpr auto kFragmentShiftDelay = 50ms;
constexpr auto kExplosionLifeTime = 1s;

Explosion::Fragment::Fragment(Asteroid *ast, primitive::Point p, primitive::Point next_p,
                              int dots_number=3) {

    this->dots_number_ = dots_number;
    this->x_shift_ = p.x - next_p.x;
    this->y_shift_ = p.y - next_p.y;
    this->angle_ = 0.1 + rand() % 15;
    this->ast = ast;

    int distribution_value = 25;
    for (int i = 0; i < this->dots_number_; ++i){
        auto rotated = p;
        rotated.rotate({p.x + rand() % distribution_value, p.y}, (rand() % 360)/M_PI);
        dots.push_back(std::move(rotated));
    }
    fragment_shift_delay_ = primitive::delay(kFragmentShiftDelay);
}

Explosion::Fragment::Fragment(Asteroid *ast, primitive::Point p1, primitive::Point p2,
                              primitive::Point p3){

    double tmp_x = p1.x/3 + p2.x/3 + p3.x/3;
    double tmp_y = p1.y/3 + p2.y/3 + p3.y/3;

    this->x_shift_ = abs(tmp_x - p1.x)/3 + abs(tmp_x - p2.x)/3 + abs(tmp_x - p3.x)/3;
    this->y_shift_ = abs(tmp_y - p1.y)/3 + abs(tmp_y - p2.y)/3 + abs(tmp_y - p3.y)/3;
    this->ast = ast;
    this->angle_ = 0.1 + rand() % 15;

    dots.push_back(p1);
    dots.push_back(p2);
    dots.push_back(p3);
    fragment_shift_delay_ = primitive::delay(kFragmentShiftDelay);
}

void Explosion::Fragment::display(SDL_Renderer *renderer, bool display_skeleton){

    int blocksize = 3;
    primitive::Point p1 = *dots.begin();
    primitive::Point p2 = *(dots.begin() + 1);
    primitive::Point p3 = *(dots.begin() + 2);
    Skeleton{renderer, ast->cg_}.update(0.0, primitive::Line{p3, p2}.length(), p2,
                   primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1, blocksize, false, true);
    shift();

}

void Explosion::Fragment::shift(){

    if (fragment_shift_delay_ > primitive::now()) { return; }

    double center_x = 0.0;
    double center_y = 0.0;

    int dots_number = dots.size();
    for(auto iter = dots.begin(); iter != dots.end(); ++iter){
        center_x += iter->x / dots_number;
        center_y += iter->y / dots_number;
        iter->x += this->x_shift_;
        iter->y += this->y_shift_;
    }

    for(auto iter = dots.begin(); iter != dots.end(); ++iter){
        iter->rotate({center_x, center_y}, angle_);
    }

    fragment_shift_delay_ = primitive::delay(kFragmentShiftDelay);
}

Explosion::Explosion(primitive::Point p, SDL_Renderer *renderer, Asteroid *ast){

    destroy_time_ = primitive::delay(kExplosionLifeTime);
    this->p_ = p;
    this->renderer_ = renderer;
    primitive::Point p_center = ast->getCenterPoint();
    this->ast = ast;

    int random_number_of_ragments = 5 + rand() % 10;
    double angle_diff = 360 / random_number_of_ragments;
    auto p_first = ast->get_points().begin();
    auto p_second = p_first + 1;

    while (p_second != ast->get_points().end()){
        fragments_.push_back(Fragment(this->ast, **p_first, **p_second, p_center));
        ++p_first;
        ++p_second;
    }
    p_second = ast->get_points().begin();
    fragments_.push_back(Fragment(this->ast, **p_first, **p_second, p_center));
}

Explosion::~Explosion(){
    delete this->ast;
}

void Explosion::display(bool display_Skeleton){
    for (auto iter = fragments_.begin(); iter != fragments_.end(); ++iter){
        iter->display(renderer_, display_Skeleton);
    }
}

bool Explosion::isAlive(){ return primitive::now() < destroy_time_; }

void Explosion::shift(primitive::Direction shift_value){

    for (auto fragment = fragments_.begin(); fragment != fragments_.end(); ++fragment){
        for(auto iter = fragment->dots.begin(); iter != fragment->dots.end(); ++iter){
            iter->move(shift_value);
        }
    }

}
