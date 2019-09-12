cmake_minimum_required(VERSION 3.10)

project(spaceGame)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(${PROJECT_NAME}
    "figure/factory_shape.cpp"
    "figure/line.cpp"
    "figure/point.cpp"
    "background.cpp"
    "colorschema.cpp"
    "common.cpp"
    "explosion.cpp"
    "game.cpp"
    "main.cpp"
    "space_ship.cpp"
    "speed.cpp"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}
    CONAN_PKG::sdl2
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
)
