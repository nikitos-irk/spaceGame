#include "Game.hpp"
#include "SpaceShip.hpp"

const int COEF = 5;
const int CITIZEN_SIZE = 5 * COEF;
const int CITIZEN_STEP = 5 * COEF;

Game::Game(SDL_Renderer *renderer){

	// Renderer
	this->renderer = renderer;

	// Create ship
	my_ship = new SpaceShip(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	
	// Create asteroids
	spaceObjects.push_back(my_ship);
	for (int i = 0; i < 10; ++i) {
		int tmp_x = rand() % SCREEN_WIDTH;
		int tmp_y = rand() % SCREEN_HEIGHT;
		int direction_x = 5 - rand() % 10;
		int direction_y = 5 - rand() % 10;
		spaceObjects.push_back(new Asteroid(renderer, tmp_x, tmp_y, direction_x, direction_y));
	}
	
	// Initiating delays
	auto now = std::chrono::system_clock::now();
	change_position_delay = now + (std::chrono::milliseconds) CHANGE_POSITION_DELAY;
	inertia_delay = now + (std::chrono::milliseconds) INERTIA_DELAY;
	
	space_pushed 	= false;
	left_pushed  	= false;
	right_pushed 	= false;
	up_pushed 		= false;
	down_pushed 	= false;

	// Inertia flags after continuous buttom pushing is over
	left_inertia  	= false;
	right_inertia 	= false;
	up_inertia		= false;
	down_inertia 	= false;

	inertia_counter_up = 0;
	inertia_counter_down = 0;
}

void Game::displayObjects(vector<SpaceObject*> &spaceObjects){
	for (auto spaceObject = spaceObjects.begin(); spaceObject != spaceObjects.end(); ++spaceObject){
		(*spaceObject)->display();
	}
}

void Game::changeObjectsPositions(vector<SpaceObject*> &spaceObjects, bool direction){

	auto now = std::chrono::system_clock::now();
	if (change_position_delay >= now){ return; }

	auto ship = spaceObjects.begin();
	DirectionXY directionXY = (dynamic_cast<SpaceShip*> (*ship))->get_direction();
	
	if (!direction){
		directionXY.x *= -1;
		directionXY.y *= -1;
	}
	for (auto spaceObject = spaceObjects.begin() + 1; spaceObject != spaceObjects.end(); ++spaceObject){
		(*spaceObject)->change_position(directionXY);
	}
	change_position_delay = std::chrono::system_clock::now() + (std::chrono::milliseconds) CHANGE_POSITION_DELAY;
}

void Game::changeObjectsPositionsByInertia(vector<SpaceObject*> &spaceObjects, bool direction){

	auto now = std::chrono::system_clock::now();
	if (inertia_delay >= now){ return; }

	auto ship = spaceObjects.begin();
	DirectionXY directionXY = (dynamic_cast<SpaceShip*> (*ship))->get_direction();
	
	if (!direction){
		directionXY.x *= -1;
		directionXY.y *= -1;
	}

	directionXY.x = directionXY.x/2;
	directionXY.y = directionXY.y/2;
	
	for (auto spaceObject = spaceObjects.begin() + 1; spaceObject != spaceObjects.end(); ++spaceObject){
		(*spaceObject)->change_position(directionXY);
	}
	inertia_delay = now + (std::chrono::milliseconds) INERTIA_DELAY;
}

void Game::fill_background(){
	int gradient_size = 1;
	int steps = 1 + (SCREEN_HEIGHT / (CITIZEN_SIZE * gradient_size));

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = CITIZEN_SIZE * 10;

	double r_start = 100;
	double g_start = 100;
	double b_start = 100;
	
	double r_finish = 0;
	double g_finish = 0;
	double b_finish = 60;

	double r = r_start;
	double g = g_start;
	double b = b_start;
	
	double r_step = abs(r_finish - r_start) / steps;
	double g_step = abs(g_finish - g_start) / steps;
	double b_step = abs(b_finish - b_start) / steps;
	
	while (rect.y < SCREEN_HEIGHT) {
		r = (r_start < r_finish) ? r + r_step : r - r_step;
		g = (g_start < g_finish) ? g + g_step : g - g_step;
		b = (b_start < b_finish) ? b + b_step : b - b_step;

		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderFillRect(renderer, &rect);
		rect.x = 0;
		rect.y += CITIZEN_SIZE * gradient_size;
	}
}

void Game::draw_grid(){
	
	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	
	for (int i = 0; i < SCREEN_HEIGHT; i = i + CITIZEN_SIZE)
		SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH, i);

	for (int i = 0; i < SCREEN_WIDTH; i = i + CITIZEN_SIZE)
		SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
}

void Game::run(){
	
	fill_background();
	displayObjects(spaceObjects);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	
	int quit = 1;

	SpaceObject *tmp_space_obj;

	while(quit) {
		while( SDL_PollEvent( &e ) != 0 ) {
			if( e.type == SDL_QUIT ){
				quit = 0;
			} else if (e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym){
			    	case SDL_QUIT:
						quit = 0;
						break;
					case SDLK_UP:
						up_pushed = true;
						changeObjectsPositions(spaceObjects, true);
						break;
					case SDLK_DOWN:
						down_pushed = true;
						changeObjectsPositions(spaceObjects, false);
						break;
					case SDLK_LEFT:
						left_pushed = true;
						my_ship->change_x(false);
						break;
					case SDLK_RIGHT:
						right_pushed = true;
						my_ship->change_x(true);
						break;
					case SDLK_SPACE:{
							space_pushed = true;
							tmp_space_obj = my_ship->shoot();
							if (nullptr != tmp_space_obj) {
								spaceObjects.push_back(tmp_space_obj);
							}
					}
						break;
					default:
						break;
				}
			} else if (e.type == SDL_KEYUP) {
				switch(e.key.keysym.sym){
					case SDLK_UP:
						up_pushed = false;
						up_inertia = true;
						inertia_counter_up = INERTIA_COUNTER;
						break;
					case SDLK_DOWN:
						down_pushed = false;
						down_inertia = true;
						inertia_counter_down = INERTIA_COUNTER;
						break;
					case SDLK_LEFT:
						left_pushed = false;
						left_inertia = true;
						break;
					case SDLK_RIGHT:
						right_pushed = false;
						right_inertia = true;
						break;
					case SDLK_SPACE:
						space_pushed = false;
						break;
					default:
						break;
				}
			}
		}

		if (down_pushed) 	{ changeObjectsPositions(spaceObjects, false); }
		if (up_pushed) 		{ changeObjectsPositions(spaceObjects, true); }
		if (left_pushed) 	{ my_ship->change_x(false); }
		if (right_pushed) 	{ my_ship->change_x(true); }
		if (space_pushed) 	{
			tmp_space_obj = my_ship->shoot();
			if (nullptr != tmp_space_obj) {
				spaceObjects.push_back(tmp_space_obj);
			}
		}
		
		if (down_inertia && inertia_counter_down) {
			changeObjectsPositionsByInertia(spaceObjects, false);
			inertia_counter_down--;
		} else {
			down_inertia = false;
		}

		if (up_inertia && inertia_counter_up) { 
			changeObjectsPositionsByInertia(spaceObjects, true); 
			inertia_counter_up--;
		} else {
			up_inertia = false;
		}
		// if (left_inertia) 	{ my_ship->change_x(false); }
		// if (right_inertia) 	{ my_ship->change_x(true); }

		fill_background();
		displayObjects(spaceObjects);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}
}