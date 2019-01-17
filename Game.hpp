#ifndef GAME_H
#define GAME_H
#include "vector"
#include "SpaceShip.hpp"
#include <random>
#include <cstdlib>
#include <ctime>
#include "SDL2/SDL.h"
#include "iostream"

using namespace std;

class Game{
private:
	std::chrono::time_point<std::chrono::system_clock> change_position_delay;
	std::chrono::time_point<std::chrono::system_clock> inertia_delay;

	// For event handling
	SDL_Event e;
	
	vector<SpaceObject*> spaceObjects;
	SDL_Renderer *renderer;
	SpaceShip *my_ship;

	// Continuous buttom pushing flags
	bool space_pushed;
	bool left_pushed;
	bool right_pushed;
	bool up_pushed;
	bool down_pushed;

	// Inertia flags after continuous buttom pushing is over
	bool left_inertia;
	bool right_inertia;
	bool up_inertia;
	bool down_inertia;

	int inertia_counter_up;
	int inertia_counter_down;

	void fill_background();
	void draw_grid();
	void displayObjects(vector<SpaceObject*>&);
	void changeObjectsPositions(vector<SpaceObject*>&, bool);
	void changeObjectsPositionsByInertia(vector<SpaceObject*>&, bool);

public:
	Game(SDL_Renderer *);
	
	void run();
};

#endif