#ifndef SPACE_PROJECTILE_HPP_
#define SPACE_PROJECTILE_HPP_

#include <memory>

#include "object.hpp"
#include "primitive/line.hpp"
#include "primitive/point.hpp"
#include "primitive/time.hpp"
#include "scene/scene.hpp"

namespace space {

class Projectile : public Object {
public:
    static constexpr auto kBlockSize{5};
    static constexpr auto kProjLifetime = 5s;
    static constexpr auto kDisplayDelay = 10ms;

    Projectile(primitive::Point coordiante, primitive::Direction d);
    bool isAlive() const { return alive_; }
    void markAsDead() { alive_ = false; }
    const primitive::Point& get_coordinate() const { return coordinate_; }
    primitive::Time const& get_life_time() const { return life_time_; }
    primitive::Line line() const { return {coordinate_, previous_}; }
    void move(primitive::Direction const& d);

    void update() override;
    void display(scene::Scene& scene) override { scene.draw(*this); }

private:
    bool alive_{true};
    primitive::Point coordinate_;
    primitive::Direction direction_;
    primitive::Point previous_;
    primitive::Time life_time_;
    primitive::Time display_delay_;
};

using ProjectilePtr = std::unique_ptr<Projectile>;

}  // namespace space

#endif /* SPACE_PROJECTILE_HPP_ */
