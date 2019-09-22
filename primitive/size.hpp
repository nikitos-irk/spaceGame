#ifndef PRIMITIVE_SIZE_HPP_
#define PRIMITIVE_SIZE_HPP_

#include <exception>

namespace primitive
{
  class Size
  {
  public:
    class NegativeSize : public std::exception {};

    constexpr Size(int w, int h) : width{w}, height{h}
    {
      if (width < 0 || height < 0) throw NegativeSize{};
    }
    Size(Size&&) = default;
    Size& operator=(Size&&) = default;
    Size(Size const&) = default;
    Size& operator=(Size const&) = default;
    ~Size() = default;
    int width{0};
    int height{0};
  };
}  // namespace primitive

#endif /* PRIMITIVE_SIZE_HPP_ */
