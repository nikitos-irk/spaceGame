#include "colorschema.hpp"

void ColorSchema::update(){
    auto now = std::chrono::system_clock::now();
    if (now <= change_colorchema_delay) { return; }
    change_colorchema_delay = now + static_cast<std::chrono::milliseconds> (COLOR_MODIFICATION_DELAY);

    cout << r << ":" << g << ":" << b << endl;
    if (flag){
        if (r < 255) { ++r; return;}
        if (g < 255) { ++g; return;}
        if (b < 255) { ++b; return;} else {flag = !flag; return;}
    } else {
        if (r > 0) { --r; return;}
        if (g > 0) { --g; return;}
        if (b > 0) { --b; return;} else { flag = !flag; return;}

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

int ColorSchema::getR(){ return r; }
int ColorSchema::getG(){ return g; }
int ColorSchema::getB(){ return b; }
