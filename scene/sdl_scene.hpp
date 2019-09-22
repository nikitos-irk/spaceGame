#ifndef SCENE_SDL_SCENE_HPP_
#define SCENE_SDL_SCENE_HPP_

#include "primitive/size.hpp"
#include "scene.hpp"

struct SDL_Renderer;

namespace scene {

class SdlScene: public Scene {
public:
    SdlScene(SDL_Renderer* renderer, primitive::Size size)
        : renderer_{renderer}, size_{std::move(size)} {}

    SdlScene(SdlScene&&) = delete;
    SdlScene& operator=(SdlScene&&) = delete;
    SdlScene(SdlScene const&) = delete;
    SdlScene& operator=(SdlScene const&) = delete;
    ~SdlScene() = default;

    void draw(space::Background* background) override;
    void draw(space::Ship* ship) override;

private:
    SDL_Renderer* renderer_{nullptr};
    primitive::Size size_;
};

}  // namespace scene

#endif /* SCENE_SDL_SCENE_HPP_ */
