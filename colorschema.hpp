#ifndef COLORSCHEMA_H
#define COLORSCHEMA_H

#include <chrono>
#include <vector>

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
    int r_, g_, b_;
    bool flag_;
    Color color_a_, color_b_;
    double color_mix_ = 1.0;
    std::chrono::time_point<std::chrono::system_clock> change_colorchema_delay_;
public:
    ColorSchema(int, int, int);
    ColorSchema(Color, Color);
    void update();
    void update(double);
    int get_r();
    int get_g();
    int get_b();
};

class ColorGenerator{
protected:
    std::vector<Color> available_colors_;
    std::vector<Color>::iterator color_iter_;
public:
    virtual ~ColorGenerator() = default;
    virtual Color getRandomColor();
    Color getNextColor();
    void setToEnd();
};

class ColorGeneratorShip: public ColorGenerator{
public:
    ColorGeneratorShip();
};

class ColorGeneratorAsteroid: public ColorGenerator{
public:
    ColorGeneratorAsteroid();
};


#endif // COLORSCHEMA_H
