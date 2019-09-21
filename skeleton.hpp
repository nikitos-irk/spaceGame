#ifndef SKELETON_HPP_
#define SKELETON_HPP_

#include <utility>

#include "primitive/point.hpp"

struct SDL_Renderer;
class ColorGenerator;

class Skeleton {
public:
    explicit Skeleton(SDL_Renderer* renderer,
                      ColorGenerator* cg)
        : renderer_{renderer},
          color_generator_{cg} {}

    void update(double angle,
        double length_of_base, primitive::Point top_point, primitive::Point down_point,
        primitive::Point pz, double block_hypotenuse, bool symmetrical, bool random_color);

private:
    std::pair<double, double> getXYOffsetOnVector(primitive::Point px1,
                                                  primitive::Point px2,
                                                  double offset_length) const;
    void fillRect(primitive::Point a, primitive::Point b, primitive::Point c);
    void putSquareOnPoint(primitive::Point center_point,
                          double block_hypotenuse_length, double angle);
    std::pair<primitive::Point, primitive::Point> getPerpendicularLineByPoint(
        primitive::Point px, primitive::Point tp1, primitive::Point tp2, double length_of_base);

    SDL_Renderer* renderer_{nullptr};
    ColorGenerator* color_generator_{nullptr};
};

#endif /* SKELETON_HPP_ */
