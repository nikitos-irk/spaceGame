#ifndef FIGURE_RECTANGLE_HPP_
#define FIGURE_RECTANGLE_HPP_

struct SDL_Rect;
struct SDL_Renderer;

namespace figure
{
  class Rectangle
  {
  public:
    Rectangle(SDL_Renderer* renderer, SDL_Rect const& rect)
        : renderer_{renderer}, rect_{rect} {}
    void fill() const;
  private:
    SDL_Renderer* renderer_;
    const SDL_Rect& rect_;
  };

}  // namespace figure

#endif /* FIGURE_RECTANGLE_HPP_ */
