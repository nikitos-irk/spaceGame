#include "colorschema.hpp"

void ColorSchema::update(){
    auto now = std::chrono::system_clock::now();
    if (now >= change_colorchema_delay) { return; }
    change_colorchema_delay = now + static_cast<std::chrono::milliseconds> (COLOR_MODIFICATION_DELAY);

    if(r > 0 && b == 0){
        r--;
        g++;
      }
      if(g > 0 && r == 0){
        g--;
        b++;
      }
      if(b > 0 && g == 0){
        r++;
        b--;
      }
}

ColorSchema::ColorSchema(int r, int g, int b){
    auto now = std::chrono::system_clock::now();
    change_colorchema_delay = now + static_cast<std::chrono::milliseconds> (COLOR_MODIFICATION_DELAY);
}

int ColorSchema::getR(){ return r; }
int ColorSchema::getG(){ return g; }
int ColorSchema::getB(){ return b; }
