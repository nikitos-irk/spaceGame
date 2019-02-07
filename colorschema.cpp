#include "colorschema.hpp"

void ColorSchema::update(){
    auto now = std::chrono::system_clock::now();
    if (now <= change_colorchema_delay) { return; }
    change_colorchema_delay = now + static_cast<std::chrono::milliseconds> (COLOR_MODIFICATION_DELAY);


    if (flag){
        if (colorMix >= 1){ flag = !flag; return;}
        colorMix += 0.01;
        r = colorA.r * (1-colorMix) + colorB.r * (colorMix);
        g = colorA.g * (1-colorMix) + colorB.g * (colorMix);
        b = colorA.b * (1-colorMix) + colorB.b * (colorMix);
    } else {
        if (colorMix <= 0){ flag = !flag; return;}
        colorMix -= 0.01;
        r = colorA.r * (1-colorMix) + colorB.r * (colorMix);
        g = colorA.g * (1-colorMix) + colorB.g * (colorMix);
        b = colorA.b * (1-colorMix) + colorB.b * (colorMix);
    }
}

ColorSchema::ColorSchema(int r, int g, int b){
    this->r = r;
    this->g = g;
    this->b = b;
    flag = false;
    auto now = std::chrono::system_clock::now();
    change_colorchema_delay = now + static_cast<std::chrono::milliseconds> (COLOR_MODIFICATION_DELAY);
}
ColorSchema::ColorSchema(Color a, Color b): colorA(a), colorB(b){}

int ColorSchema::getR(){ return r; }
int ColorSchema::getG(){ return g; }
int ColorSchema::getB(){ return b; }
