#ifndef FIGURE_LINE_HPP_
#define FIGURE_LINE_HPP_

struct SDL_Renderer;

namespace figure
{
class Line {
public:
    Line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
        : renderer_{renderer}, x1_{x1}, y1_{y1}, x2_{x2}, y2_{y2} {}
    void draw() const;
private:
    SDL_Renderer* renderer_{nullptr};
    int x1_{0};
    int y1_{0};
    int x2_{0};
    int y2_{0};
};

}  // namespace figure

#endif /* FIGURE_LINE_HPP_ */
