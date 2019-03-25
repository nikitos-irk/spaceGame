#ifndef GAME_H
#define GAME_H
#include "vector"
#include "list"
#include "SpaceShip.hpp"
#include <random>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "SDL2/SDL.h"
#include "iostream"
#include "Background.hpp"
#include "Background.hpp"
#include "thread"
#include "explosion.hpp"

static std::exception_ptr globalExceptionPtr = nullptr;

#define ASTEROIDS_REMOVING_DELAY 10000 // 10 seconds
using namespace std;

class GameOverException: public exception{
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
	
	vector<SpaceObject*> spaceObjects;
    list<SpaceObject*> asteroids;
    list<SpaceObject*> projectiles;
    list<Explosion*> explosions;
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

    void displayObjects();
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
