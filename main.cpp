#include <iostream>

#include "application.hpp"

int main()
try {
    constexpr int kScreenWidth = 640 * 2;
    constexpr int kScreenHeight = 480 * 2;

    Application{primitive::Size{kScreenWidth, kScreenHeight}}.run();
}
catch (SdlError const& error) {
    std::cout << error.what() << '\n';
    return -1;
}
catch (...) {
    std::cout << "Unknown error\n";
    return -1;
}
