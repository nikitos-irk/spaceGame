#ifndef COLORSCHEMA_H
#define COLORSCHEMA_H

#include <initializer_list>
#include <vector>

#include "primitive/color.hpp"
#include "primitive/time.hpp"

class ColorSchema{
    const primitive::Color color_a_;
    const primitive::Color color_b_;
    primitive::Color color_;
    primitive::Time change_colorchema_delay_;
    double color_mix_{1.0};
    bool flag_{false};

public:
    ColorSchema(primitive::Color, primitive::Color);
    void update();
    void update(double);
    primitive::Color get_color() const { return color_; }
};

class ColorGenerator {
public:
    explicit ColorGenerator(std::initializer_list<primitive::Color> colors)
        : available_colors_{colors},
          color_iter_{available_colors_.end()} {}
    virtual ~ColorGenerator() = default;
    virtual primitive::Color getRandomColor();
    primitive::Color getNextColor();
    void setToEnd();

private:
    std::vector<primitive::Color> available_colors_;
    std::vector<primitive::Color>::iterator color_iter_;
};

class ColorGeneratorShip: public ColorGenerator {
public:
    ColorGeneratorShip() : ColorGenerator{primitive::Color{220,220,220},
                                          primitive::Color{192,192,192},
                                          primitive::Color{105,105,105},
                                          primitive::Color{211,211,211},
                                          primitive::Color{119,136,153}} {}
};

class ColorGeneratorAsteroid: public ColorGenerator {
public:
    ColorGeneratorAsteroid() : ColorGenerator{primitive::Color{160, 177, 188},
                                              primitive::Color{119, 98, 84},
                                              primitive::Color{128, 0, 0},
                                              primitive::Color{81, 81, 81},
                                              primitive::Color{76, 62, 54},
                                              primitive::Color{139, 69, 19}} {}
};


#endif // COLORSCHEMA_H
