#ifndef SCENE_SCENE_HPP_
#define SCENE_SCENE_HPP_

namespace space {
class Background;
class Grid;
class LifeAmount;
class Nozzle;
class Projectile;
class Ship;
}  // namespace space

namespace scene {

class Scene {
public:
    virtual ~Scene() = default;
    virtual void draw(space::Background const& background) = 0;
    virtual void draw(space::Grid const& grid) = 0;
    virtual void draw(space::Ship const& ship) = 0;
    virtual void draw(space::Nozzle const& nozzle) = 0;
    virtual void draw(space::Projectile const& projectile) = 0;
    virtual void draw(space::LifeAmount const& lifes) = 0;
};

}  // namespace scene

#endif /* SCENE_SCENE_HPP_ */
