#ifndef FIGURE_POINT_HPP_
#define FIGURE_POINT_HPP_

struct SDL_Renderer;

namespace figure
{
class Point
{
public:
    Point(SDL_Renderer* renderer, int x, int y)
        : renderer_{renderer}, x_{x}, y_{y} {}
    void draw() const;
private:
    SDL_Renderer* renderer_{nullptr};
    int x_{0};
    int y_{0};
};
}  // namespace figure

#endif /* FIGURE_POINT_HPP_ */
