#ifndef SPACE_SHIP_HPP_
#define SPACE_SHIP_HPP_

#include <vector>

#include "colorschema.hpp"
#include "object.hpp"
#include "nozzle.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "scene/scene.hpp"

namespace space {

class Ship: public Object {
public:
  using Border = std::vector<primitive::Point>;

  static constexpr primitive::Size size{30, 60};
  const ColorSchema colors{{255, 255, 0}, {255,8,0}};

  explicit Ship(primitive::Point coordinate);
  Border const& get_border() const { return border_; }
  Nozzle const& get_left_nozzle() const { return left_nozzle_; }
  Nozzle const& get_right_nozzle() const { return right_nozzle_; }
  double getTiltAngel() const;

  void display(scene::Scene& scene) override { scene.draw(*this); }

private:
  Border border_;
  Nozzle left_nozzle_;
  Nozzle right_nozzle_;

};

}  // namespace space

#endif /* SPACE_SHIP_HPP_ */
