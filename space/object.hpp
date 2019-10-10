#ifndef SPACE_OBJECT_HPP_
#define SPACE_OBJECT_HPP_

namespace scene {
class Scene;
}  // namespace scene

namespace space {

class Object {
public:
    virtual ~Object() = default;
    virtual void update() = 0;
    virtual void display(scene::Scene& scene) const = 0;
};

}  // namespace

#endif /* SPACE_OBJECT_HPP_ */
