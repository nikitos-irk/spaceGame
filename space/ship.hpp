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
#include "speed.hpp"

namespace space {

class Ship: public Object {
public:
  using Border = std::vector<primitive::Point>;

  static constexpr double kAngle{M_PI/15};
  static constexpr auto kRotationDelay = 60ms;
  static constexpr int kMaxSpeed{50};
  static constexpr primitive::Size size{30, 60};
  const ColorSchema colors{{255, 255, 0}, {255,8,0}};

  explicit Ship(primitive::Point coordinate);
  Border const& get_border() const { return border_; }
  Nozzle const& get_left_nozzle() const { return left_nozzle_; }
  Nozzle const& get_right_nozzle() const { return right_nozzle_; }
  Speed const& get_speed() const { return speed_; }
  primitive::Point const& get_initial_median_intersaction() const
  { return initial_median_intersection_; }
  double getTiltAngel() const;
  void slowdown();
  void accelarate();
  void backwardSlowdown();
  void backwardAccelarate();
  void rotate(bool clockwise);
  void rotatePointsInVector(std::vector<primitive::Point> &points,
                            primitive::Point initial_median_intersection,
                            double angle) const;

  void update() override;
  void display(scene::Scene& scene) override { scene.draw(*this); }

private:
  primitive::Point CalcMedianIntersaction() const;
  Border border_;
  Nozzle left_nozzle_;
  Nozzle right_nozzle_;
  Speed speed_{kMaxSpeed};
  primitive::Time rotation_delay_;
  primitive::Point initial_median_intersection_;
};

}  // namespace space

#endif /* SPACE_SHIP_HPP_ */
