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

void ColorSchema::update(double currentA){
    auto now = std::chrono::system_clock::now();
    if (now <= change_colorchema_delay) { return; }
    change_colorchema_delay = now + static_cast<std::chrono::milliseconds> (COLOR_MODIFICATION_DELAY);

    r = colorA.r + abs(colorA.r - colorB.r) * currentA;
    g = colorA.g + abs(colorA.g - colorB.g) * currentA;
    b = colorA.b + abs(colorA.b - colorB.b) * currentA;
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

colorGeneratorShip::colorGeneratorShip(){
    availableColors.push_back(Color(220,220,220));
    availableColors.push_back(Color(192,192,192));
    availableColors.push_back(Color(105,105,105));
    availableColors.push_back(Color(211,211,211));
    availableColors.push_back(Color(119,136,153));
    colorIter = availableColors.end();
}

colorGeneratorAsteroid::colorGeneratorAsteroid(){
    availableColors.push_back(Color(96, 54, 102));
    availableColors.push_back(Color(209, 167, 229));
    availableColors.push_back(Color(176, 10, 255));
    availableColors.push_back(Color(72, 6, 104));
    availableColors.push_back(Color(51, 9, 71));
    availableColors.push_back(Color(48, 23, 61));
    availableColors.push_back(Color(119, 98, 130));
    availableColors.push_back(Color(217, 204, 224));
    availableColors.push_back(Color(205, 114, 255));
    colorIter = availableColors.end();
}

Color colorGenerator::getNextColor(){
    Color result;
    if (colorIter == availableColors.end()) { colorIter = availableColors.begin();}
    result = *colorIter;
    colorIter++;
    return result;
}

void colorGenerator::setToEnd(){
    colorIter = availableColors.end();
}

Color colorGenerator::getRandomColor(){
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
