#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <exception>
#include <list>
#include <mutex>
#include <vector>

#include <SDL2/SDL.h>

#include "primitive/size.hpp"

class Asteroid;
class Background;
class Explosion;
class SpaceShip;
class SpaceObject;

static std::exception_ptr globalExceptionPtr = nullptr;

class GameOverException: public std::exception {
    const char *file_;
    int line_;
    const char* func_;
    const char* info_;
public:
    GameOverException(const char *msg, const char *file, int line, const char *func, const char *info=""):
        file_(file),
        line_(line),
        func_(func),
        info_(info)
    {}
    GameOverException(){}
    const char * get_file() {return file_;}
    int get_line() {return line_;}
    const char * get_func() {return func_;}
    const char * get_info() {return info_;}
};

class Game{
private:
    SDL_Renderer *renderer_{nullptr};
    primitive::Size screen_size_;
    int live_amount_;

    std::chrono::time_point<std::chrono::system_clock> change_position_delay_;
    std::chrono::time_point<std::chrono::system_clock> inertia_delay_;
    std::chrono::time_point<std::chrono::system_clock> update_asteroids_delay_;

    std::mutex asteroids_mutex_;
    std::mutex projectiles_mutex_;

    // Background class
    Background *my_background_{nullptr};

    // For event handling
    SDL_Event e_;

    std::vector<SpaceObject*> space_objects_;
    std::list<SpaceObject*> asteroids_;
    std::list<SpaceObject*> projectiles_;
    std::list<Explosion*> explosions_;

    SpaceShip *my_ship_{nullptr};

    // Continuous buttom pushing flags
    bool space_pushed_{false};
    bool left_pushed_{false};
    bool right_pushed_{false};
    bool up_pushed_{false};
    bool up_unpushed_{false};
    bool down_pushed_{false};
    bool down_unpushed_{false};

    int inertia_counter_up_{0};
    int inertia_counter_down_{0};

    void displayObjects(bool);
    void changeObjectsPositions();

    void createAsteroid();
    void updateAsteroids();
    void updateProjectiles();

    void checkHits();
    void histLoop();
    void cleanAsteroids();
    void cleanProjectiles();
    void cleanExplosions();
    void cleanLoop();

    void checkShipHits();
    void shipHitsLoop();
    void update();
    void displayLifeAmount();

    void generateExplosion(Asteroid*);
public:
    Game(SDL_Renderer*, primitive::Size, int);
    void run();
};

#endif
