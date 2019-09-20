#ifndef COLORSCHEMA_H
#define COLORSCHEMA_H

#include <vector>

#include "primitive/color.hpp"
#include "primitive/time.hpp"

class ColorSchema{
    int r_, g_, b_;
    bool flag_;
    primitive::Color color_a_, color_b_;
    double color_mix_ = 1.0;
    primitive::Time change_colorchema_delay_;
public:
    ColorSchema(int, int, int);
    ColorSchema(primitive::Color, primitive::Color);
    void update();
    void update(double);
    int get_r();
    int get_g();
    int get_b();
};

class ColorGenerator{
protected:
    std::vector<primitive::Color> available_colors_;
    std::vector<primitive::Color>::iterator color_iter_;
public:
    virtual ~ColorGenerator() = default;
    virtual primitive::Color getRandomColor();
    primitive::Color getNextColor();
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
