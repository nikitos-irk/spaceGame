#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include <memory>
#include <vector>

#include "colorschema.hpp"

#include "object.hpp"
#include "primitive/point.hpp"
#include "primitive/time.hpp"
#include "scene/scene.hpp"

namespace space {
class Asteroid;

class Fragment : Object {
public:
    static constexpr auto kFragmentShiftDelay = 50ms;

    Fragment(ColorGenerator cg, primitive::Point, primitive::Point, primitive::Point);
    void update() override;
    void display(scene::Scene& scene) const override { scene.draw(*this); }

    std::vector<primitive::Point> dots;
    ColorGenerator colors;

private:
    primitive::Time fragment_shift_delay_;
    double x_shift_;
    double y_shift_;
    primitive::Point next_p_;
    double angle_;
};

class Explosion : public Object {
public:
    using Fragments = std::vector<Fragment>;
    static constexpr auto kExplosionLifeTime = 1s;

    Explosion(primitive::Point p, space::Asteroid const&);
    void move(primitive::Direction);
    bool isAlive() const { return primitive::now() < destroy_time_; }
    const Fragments& fragments() const { return fragments_; }

    void update() override;
    void display(scene::Scene& scene) const override { scene.draw(*this); }

private:
    primitive::Point p_;
    primitive::Time destroy_time_;
    Fragments fragments_;
};

using ExplosionPtr = std::unique_ptr<Explosion>;

}  // namespace space

#endif // EXPLOSION_HPP
