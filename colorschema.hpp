#ifndef COLORSCHEMA_H
#define COLORSCHEMA_H

//#include <cstdlib>
//#include <ctime>
//#include <unistd.h>
//#include "SDL2/SDL.h"
#include "iostream"
#define COLOR_MODIFICATION_DELAY 1000

using namespace std;

class ColorSchema{
    int r, g, b;
    std::chrono::time_point<std::chrono::system_clock> change_colorchema_delay;
public:
    ColorSchema(int, int, int);
    void update();
    int getR();
    int getG();
    int getB();
};
#endif // COLORSCHEMA_H
