#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "object.hpp"
#include "primitive/color.hpp"
#include "scene/scene.hpp"

namespace space {
// Hope I will think something more interesting background than it is now
class Background : public Object {
public:
    static constexpr primitive::Color gradient_start{100, 100, 100};
    static constexpr primitive::Color gradient_finish{0, 0, 60};
    static constexpr primitive::Color grid_color{192, 192, 192, 255};
    bool grid{false};

    void display(scene::Scene& scene) override { scene.draw(this); }
};

}  // namespace space

#endif
