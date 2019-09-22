#ifndef SPACE_GRID_HPP_
#define SPACE_GRID_HPP_

#include "object.hpp"
#include "primitive/color.hpp"
#include "scene/scene.hpp"

namespace space {
class Grid : public Object {
public:
    static constexpr primitive::Color color{192, 192, 192, 255};
    static constexpr auto cell_size{25};

    void display(scene::Scene& scene) override { scene.draw(*this); }
};
}  // namespace space

#endif /* SPACE_GRID_HPP_ */
