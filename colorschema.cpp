#include "colorschema.hpp"

#include <cstdlib>
#include <iostream>

constexpr auto kColorModificationDelay = 10ms;

ColorSchema::ColorSchema(primitive::Color a, primitive::Color b, primitive::Color c)
    : color_a_(a), color_b_(b), color_c_(c) {}

void ColorSchema::update_(double current_a){
    if (primitive::now() <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = primitive::delay(kColorModificationDelay);

    primitive::Color tmp;
    int multiply_red_{1};
    int multiply_green_{1};
    int multiply_blue_{1};
    
    if (current_a < 0) {
        tmp = color_b_;
        if (color_a_.red < tmp.red){ multiply_red_ = -1;} else { multiply_red_ = 1;}
        if (color_a_.green < tmp.green){ multiply_green_ = -1;} else { multiply_green_ = 1;}
        if (color_a_.blue < tmp.blue){ multiply_blue_ = -1;} else { multiply_blue_ = 1;}
    } else {
        tmp = color_c_;
        if (color_a_.red < tmp.red){ multiply_red_ = 1;} else { multiply_red_ = -1;}
        if (color_a_.green < tmp.green){ multiply_green_ = 1;} else { multiply_green_ = -1;}
        if (color_a_.blue < tmp.blue){ multiply_blue_ = 1;} else { multiply_blue_ = -1;}
    }

    color_.red = color_a_.red + std::abs(color_a_.red - tmp.red) * current_a * multiply_red_;
    color_.green = color_a_.green + std::abs(color_a_.green - tmp.green) * current_a * multiply_green_;
    color_.blue = color_a_.blue + std::abs(color_a_.blue - tmp.blue) * current_a * multiply_blue_;
}

void ColorSchema::update(double current_a){
    if (primitive::now() <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = primitive::delay(kColorModificationDelay);

    color_.red = color_a_.red + std::abs(color_a_.red - color_b_.red) * current_a;
    color_.green = color_a_.green + std::abs(color_a_.green - color_b_.green) * current_a;
    color_.blue = color_a_.blue + std::abs(color_a_.blue - color_b_.blue) * current_a;
}
