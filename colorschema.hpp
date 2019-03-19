#ifndef COLORSCHEMA_H
#define COLORSCHEMA_H

#include "iostream"
#include <complex>
#define COLOR_MODIFICATION_DELAY 10

using namespace std;

struct Color{
    int r, g, b;
    Color(int r, int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Color(){
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
    Color& operator=(const Color &a){
        if (this != &a) {
            r = a.r;
            g = a.g;
            b = a.b;
        }
        return *this;
    }
};

class ColorSchema{
    int r, g, b;
    bool flag;
    Color colorA, colorB;
    double colorMix = 1.0;
    std::chrono::time_point<std::chrono::system_clock> change_colorchema_delay;
public:
    ColorSchema(int, int, int);
    ColorSchema(Color, Color);
    void update();
    void update(double);
    int getR();
    int getG();
    int getB();
};
#endif // COLORSCHEMA_H
