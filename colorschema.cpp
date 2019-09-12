#include "colorschema.hpp"

#include <cstdlib>

using namespace std::chrono;

constexpr auto kColorModificationDelay = 10ms;

void ColorSchema::update(){
    auto now = std::chrono::system_clock::now();
    if (now <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = now + kColorModificationDelay;

    if (flag_){
        if (color_mix_ >= 1){ flag_ = !flag_; return;}
        color_mix_ += 0.01;
        r_ = color_a_.r * (1-color_mix_) + color_b_.r * (color_mix_);
        g_ = color_a_.g * (1-color_mix_) + color_b_.g * (color_mix_);
        b_ = color_a_.b * (1-color_mix_) + color_b_.b * (color_mix_);
    } else {
        if (color_mix_ <= 0){ flag_ = !flag_; return;}
        color_mix_ -= 0.01;
        r_ = color_a_.r * (1-color_mix_) + color_b_.r * (color_mix_);
        g_ = color_a_.g * (1-color_mix_) + color_b_.g * (color_mix_);
        b_ = color_a_.b * (1-color_mix_) + color_b_.b * (color_mix_);
    }
}

void ColorSchema::update(double current_a){
    auto now = std::chrono::system_clock::now();
    if (now <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = now + kColorModificationDelay;

    r_ = color_a_.r + abs(color_a_.r - color_b_.r) * current_a;
    g_ = color_a_.g + abs(color_a_.g - color_b_.g) * current_a;
    b_ = color_a_.b + abs(color_a_.b - color_b_.b) * current_a;
}

ColorSchema::ColorSchema(int r, int g, int b){
    this->r_ = r;
    this->g_ = g;
    this->b_ = b;
    flag_ = false;
    auto now = std::chrono::system_clock::now();
    change_colorchema_delay_ = now + kColorModificationDelay;
}

ColorSchema::ColorSchema(Color a, Color b): color_a_(a), color_b_(b){}

int ColorSchema::get_r(){ return r_; }
int ColorSchema::get_g(){ return g_; }
int ColorSchema::get_b(){ return b_; }

ColorGeneratorShip::ColorGeneratorShip(){
    available_colors_.push_back(Color(220,220,220));
    available_colors_.push_back(Color(192,192,192));
    available_colors_.push_back(Color(105,105,105));
    available_colors_.push_back(Color(211,211,211));
    available_colors_.push_back(Color(119,136,153));
    color_iter_ = available_colors_.end();
}

ColorGeneratorAsteroid::ColorGeneratorAsteroid(){
    available_colors_.push_back(Color(160, 177, 188));
    available_colors_.push_back(Color(119, 98, 84));
    available_colors_.push_back(Color(128, 0, 0));
    available_colors_.push_back(Color(81, 81, 81));
    available_colors_.push_back(Color(76, 62, 54));
    available_colors_.push_back(Color(139, 69, 19));
    color_iter_ = available_colors_.end();
}

Color ColorGenerator::getNextColor(){
    Color result;
    if (color_iter_ == available_colors_.end()) { color_iter_ = available_colors_.begin();}
    result = *color_iter_;
    color_iter_++;
    return result;
}

void ColorGenerator::setToEnd(){
    color_iter_ = available_colors_.end();
}

Color ColorGenerator::getRandomColor(){
    Color randomColor;
    switch (rand() % 5){
        case 1: randomColor = Color(220,220,220); break;
        case 2: randomColor = Color(192,192,192); break;
        case 3: randomColor = Color(105,105,105); break;
        case 4: randomColor = Color(211,211,211); break;
        case 5: randomColor = Color(119,136,153); break;
        default: break;
    }
    return randomColor;
}
