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
#include "Inertia.hpp"
#include "speed.hpp"

using namespace std;

class Game{
private:
    Speed *sp;
	int screen_width;
	int screen_height;
	std::chrono::time_point<std::chrono::system_clock> change_position_delay;
	std::chrono::time_point<std::chrono::system_clock> inertia_delay;

	// Background class
	Background *my_background;
	
	// Inertia vector
	list<Inertia*> inertias;

	// For event handling
	SDL_Event e;
	
	vector<SpaceObject*> spaceObjects;
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

	void displayObjects(vector<SpaceObject*>&);
    void changeObjectsPositions(vector<SpaceObject*>&);
	void changeObjectsPositionsByInertia(vector<SpaceObject*>&, bool);

public:
	Game(SDL_Renderer *, int, int);
	
	void run();
};

#endif
