#ifndef FIGURE_EXCEPTION_HPP_
#define FIGURE_EXCEPTION_HPP_

#include <exception>

namespace figure {
class ErrorDrawing : public std::exception {};
class BadRenderer : public std::exception {};
}

#endif /* FIGURE_EXCEPTION_HPP_ */
