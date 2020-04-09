#include "projectile.hpp"

namespace space {
Projectile::Projectile(primitive::Point coordinate, primitive::Direction d)
    : coordinate_{std::move(coordinate)}, direction_{d}
{
    life_time_ = primitive::delay(kProjLifetime);
    display_delay_ = primitive::delay(kDisplayDelay);
}

void Projectile::move(primitive::Direction const& d)
{
  previous_ = coordinate_;
  coordinate_.move(d);
}

void Projectile::update()
{
    if (display_delay_ > primitive::now()){ return; }
    coordinate_.move(direction_);
    display_delay_ = primitive::delay(kDisplayDelay/5);
}
}  // namespace space
