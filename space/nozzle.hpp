#ifndef SPACE_NOZZLE_HPP_
#define SPACE_NOZZLE_HPP_

#include <vector>

#include "colorschema.hpp"
#include "object.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "scene/scene.hpp"

namespace space {

class Nozzle: public Object {
public:
  using Border = std::vector<primitive::Point>;

  static constexpr primitive::Size size{15, 25};
  ColorSchema colors{{255, 17, 0}, {255, 237, 0}};

  Nozzle(Ship const& ship, primitive::Point coordinate);
  Border const& get_border() const { return border_; }
  double getTiltAngel() const;

  void update() override;
  void display(scene::Scene& scene) override { scene.draw(*this); }

private:
  Ship const& ship_;
  Border border_;
  Border border_origin_;
};

}  // namespace space

#endif /* SPACE_NOZZLE_HPP_ */
