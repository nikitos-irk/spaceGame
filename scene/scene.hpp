#ifndef SCENE_SCENE_HPP_
#define SCENE_SCENE_HPP_

namespace space {
class Background;
class Ship;
}  // namespace space

namespace scene {

class Scene {
public:
    virtual ~Scene() = default;
    virtual void draw(space::Background* background) = 0;
    virtual void draw(space::Ship* ship) = 0;
};

}  // namespace scene

#endif /* SCENE_SCENE_HPP_ */
