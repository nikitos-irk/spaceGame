#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <exception>
#include <list>
#include <mutex>
#include <vector>

#include <SDL2/SDL.h>

class Asteroid;
class Background;
class Explosion;
class SpaceShip;
class SpaceObject;

static std::exception_ptr globalExceptionPtr = nullptr;

constexpr auto ASTEROIDS_REMOVING_DELAY = std::chrono::seconds(10);

class GameOverException: public std::exception {
    const char *file;
    int line;
    const char* func;
    const char* info;
public:
    GameOverException(const char *msg, const char *file_, int line_, const char *func_, const char *info_=""):
        file(file_),
        line(line_),
        func(func_),
        info(info_)
    {}
    GameOverException(){}
    const char * get_file() {return file;}
    int get_line() {return line;}
    const char * get_func() {return func;}
    const char * get_info() {return info;}
};

class Game{
private:

    int screen_width;
	int screen_height;
    int liveAmount;

	std::chrono::time_point<std::chrono::system_clock> change_position_delay;
	std::chrono::time_point<std::chrono::system_clock> inertia_delay;
    std::chrono::time_point<std::chrono::system_clock> update_asteroids_delay;

    std::mutex asteroids_mutex;
    std::mutex projectiles_mutex;

	// Background class
	Background *my_background;

	// For event handling
	SDL_Event e;
	
  std::vector<SpaceObject*> spaceObjects;
  std::list<SpaceObject*> asteroids;
  std::list<SpaceObject*> projectiles;
  std::list<Explosion*> explosions;
	SDL_Renderer *renderer;
	SpaceShip *my_ship;

	// Continuous buttom pushing flags
	bool space_pushed;
	bool left_pushed;
	bool right_pushed;
    bool up_pushed, up_unpushed;
    bool down_pushed, down_unpushed;

	int inertia_counter_up;
	int inertia_counter_down;

    void displayObjects(bool);
    void changeObjectsPositions();

    void create_asteroid();
    void update_asteroids();
    void update_projectiles();

    void check_hits();
    void hist_loop();
    void clean_asteroids();
    void clean_projectiles();
    void clean_explosions();
    void clean_loop();

    void check_ship_hits();
    void ship_hits_loop();
    void update();
    void displayLifeAmount();

    void generate_explosion(Asteroid*);
public:
    Game(SDL_Renderer *, int, int, int);
	void run();
};

#endif
