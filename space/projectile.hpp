#ifndef SPACE_PROJECTILE_HPP_
#define SPACE_PROJECTILE_HPP_

#include <memory>

#include "object.hpp"
#include "scene/scene.hpp"

namespace space {

class Projectile : public Object {
public:
    void update() override {}
    void display(scene::Scene& scene) override { scene.draw(*this); }
};

using ProjectilePtr = std::unique_ptr<Projectile>;

}  // namespace space

#endif /* SPACE_PROJECTILE_HPP_ */
