#include "colorschema.hpp"

#include <cstdlib>

constexpr auto kColorModificationDelay = 10ms;

ColorSchema::ColorSchema(primitive::Color a, primitive::Color b)
    : color_a_(a), color_b_(b) {}

void ColorSchema::update(){
    if (primitive::now() <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = primitive::delay(kColorModificationDelay);

    if (flag_){
        if (color_mix_ >= 1){ flag_ = !flag_; return;}
        color_mix_ += 0.01;
        color_.red = color_a_.red * (1-color_mix_) + color_b_.red * (color_mix_);
        color_.green = color_a_.green * (1-color_mix_) + color_b_.green * (color_mix_);
        color_.blue = color_a_.blue * (1-color_mix_) + color_b_.blue * (color_mix_);
    } else {
        if (color_mix_ <= 0){ flag_ = !flag_; return;}
        color_mix_ -= 0.01;
        color_.red = color_a_.red * (1-color_mix_) + color_b_.red * (color_mix_);
        color_.green = color_a_.green * (1-color_mix_) + color_b_.green * (color_mix_);
        color_.blue = color_a_.blue * (1-color_mix_) + color_b_.blue * (color_mix_);
    }
}

void ColorSchema::update(double current_a){
    if (primitive::now() <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = primitive::delay(kColorModificationDelay);

    color_.red = color_a_.red + std::abs(color_a_.red - color_b_.red) * current_a;
    color_.green = color_a_.green + std::abs(color_a_.green - color_b_.green) * current_a;
    color_.blue = color_a_.blue + std::abs(color_a_.blue - color_b_.blue) * current_a;
}


primitive::Color ColorGenerator::getNextColor(){
  primitive::Color result;
    if (color_iter_ == available_colors_.end()) { color_iter_ = available_colors_.begin();}
    result = *color_iter_;
    color_iter_++;
    return result;
}

void ColorGenerator::setToEnd(){
    color_iter_ = available_colors_.end();
}

primitive::Color ColorGenerator::getRandomColor(){
    primitive::Color randomColor;
    switch (rand() % 5){
        case 1: randomColor = primitive::Color{220,220,220}; break;
        case 2: randomColor = primitive::Color{192,192,192}; break;
        case 3: randomColor = primitive::Color{105,105,105}; break;
        case 4: randomColor = primitive::Color{211,211,211}; break;
        case 5: randomColor = primitive::Color{119,136,153}; break;
        default: break;
    }
    return randomColor;
}
