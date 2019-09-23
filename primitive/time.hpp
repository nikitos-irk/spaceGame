#ifndef PRIMITIVE_TIME_HPP_
#define PRIMITIVE_TIME_HPP_

#include <chrono>

using namespace std::literals::chrono_literals;

namespace primitive {

using Time = std::chrono::time_point<std::chrono::system_clock>;

inline Time now() {
  return std::chrono::system_clock::now();
}

template<class Rep, class Period>
inline Time delay(const std::chrono::duration<Rep, Period>& d) {
  return now() + d;
}

}  // namespace primitive

#endif /* PRIMITIVE_TIME_HPP_ */
