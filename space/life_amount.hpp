#ifndef SPACE_LIFE_AMOUNT_HPP_
#define SPACE_LIFE_AMOUNT_HPP_

#include "object.hpp"
#include "primitive/size.hpp"
#include "scene/scene.hpp"

namespace space {

class LifeAmount : public Object {
public:
    const primitive::Size kSize{5 * 15 + 4, 24};
    const primitive::Size kBarSize{10, 20};

    explicit LifeAmount(int amount) : amount_{amount} {}
    int get_amount() const { return amount_; }
    operator bool() { return amount_ > 0; }
    LifeAmount& operator--(int) { --amount_; return *this; }

    void update() override {}
    void display(scene::Scene& scene) override { scene.draw(*this); }

private:
    int amount_;
};

}  // namespace space

#endif /* SPACE_LIFE_AMOUNT_HPP_ */
