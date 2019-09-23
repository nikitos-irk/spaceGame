#ifndef FIGURE_POLYGON_HPP_
#define FIGURE_POLYGON_HPP_

#include <utility>
#include <vector>

struct SDL_Renderer;

namespace figure {

class Polygon {
public:
    using Points = std::vector<std::pair<int, int>>;
    Polygon(SDL_Renderer* renderer, Points const& points)
        : renderer_{renderer}, points_{points} {}
    void draw() const;

private:
    void drawLine(int x1, int y1, int x2, int y2) const;
    SDL_Renderer* renderer_;
    Points points_;
};

}  // namespace figure

#endif /* FIGURE_POLYGON_HPP_ */
