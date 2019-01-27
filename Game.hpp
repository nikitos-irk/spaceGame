#ifndef GAME_H
#define GAME_H
#include "vector"
#include "list"
#include "SpaceShip.hpp"
#include <random>
#include <cstdlib>
#include <ctime>
#include "SDL2/SDL.h"
#include "iostream"
#include "Background.hpp"
#include "Background.hpp"

#define ASTEROIDS_REMOVING_DELAY 10000 // 10 seconds
using namespace std;

class Game{
private:
	int screen_width;
	int screen_height;
	std::chrono::time_point<std::chrono::system_clock> change_position_delay;
	std::chrono::time_point<std::chrono::system_clock> inertia_delay;
    std::chrono::time_point<std::chrono::system_clock> update_asteroids_delay;

	// Background class
	Background *my_background;

	// For event handling
	SDL_Event e;
	
	vector<SpaceObject*> spaceObjects;
    list<SpaceObject*> asteroids;
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

public:
	Game(SDL_Renderer *, int, int);
	
	void run();
};

#endif
