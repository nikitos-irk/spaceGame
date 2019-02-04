#ifndef COLORSCHEMA_H
#define COLORSCHEMA_H

#include "iostream"
#include <complex>
#define COLOR_MODIFICATION_DELAY 10

using namespace std;

class ColorSchema{
    int r, g, b;
    bool flag;
    std::chrono::time_point<std::chrono::system_clock> change_colorchema_delay;
public:
    ColorSchema(int, int, int);
    void update();
    int getR();
    int getG();
    int getB();
};
#endif // COLORSCHEMA_H
