#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <exception>
#include <list>
#include <mutex>
#include <vector>

#include <SDL2/SDL.h>

#include "primitive/size.hpp"
#include "primitive/time.hpp"
#include "scene/sdl_scene.hpp"
#include "space/background.hpp"
#include "space/ship.hpp"
#include "space_ship.hpp"

class Asteroid;
class Background;
class Explosion;
class SpaceShip;
class SpaceObject;

struct SDL_Renderer;

static std::exception_ptr globalExceptionPtr = nullptr;

class GameOverException: public std::exception {
    const char *file_{nullptr};
    int line_{0};
    const char* func_{nullptr};
    const char* info_{nullptr};
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
    std::atomic_bool running_{false};

    primitive::Time change_position_delay_;
    primitive::Time inertia_delay_;
    primitive::Time update_asteroids_delay_;

    std::mutex asteroids_mutex_;
    std::mutex projectiles_mutex_;

    // For event handling
    SDL_Event e_;

    space::Background background_;
    space::Ship ship_;
    SpaceShip my_ship_;
    std::vector<SpaceObject*> space_objects_;
    std::list<SpaceObject*> asteroids_;
    std::list<SpaceObject*> projectiles_;
    std::list<Explosion*> explosions_;

    scene::SdlScene scene_;

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

    void displayObjects();
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
