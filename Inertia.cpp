#include "Inertia.hpp"

Inertia::Inertia(SpaceShip* spaceShip, vector<SpaceObject*> &spaceObjects): spaceObjects(spaceObjects), directionXY(){
	this->spaceObjects = spaceObjects;
	this->directionXY = spaceShip->get_direction();
	this->alive = true;
	this->persantage = 100;
	this->direction_v = 0;
}

bool Inertia::isAlive(){ return alive; }

void Inertia::run(){}
Inertia::~Inertia(){}

InertiaUpDown::InertiaUpDown(SpaceShip* spaceShip, vector<SpaceObject*> &spaceObjects, bool forwardDirection) : Inertia(spaceShip, spaceObjects){
	this->forwardDirection = forwardDirection;
}
InertiaUpDown::~InertiaUpDown(){}

InertiaRotation::InertiaRotation(SpaceShip* spaceShip, vector<SpaceObject*> &spaceObjects, bool clockwiseDirection) : Inertia(spaceShip, spaceObjects){
	this->clockwiseDirection = clockwiseDirection;
}
InertiaRotation::~InertiaRotation(){}

void InertiaUpDown::run(){
	auto now = std::chrono::system_clock::now();
	if (inertia_delay >= now && direction_v <= 0){ return; }
	
	long d = sqrt(pow(directionXY.x, 2) + pow(directionXY.y, 2));
	d -= pow(directionXY.x, 3);
	
	//angel between directional vector and base axis offset
	float x_0 = directionXY.x, x_1 = directionXY.x;
	float y_0 = directionXY.y, y_1 = directionXY.y;
	
	directionXY.x = pow(directionXY.x, 2/3);
	directionXY.y = pow(directionXY.y, 2/3);
	cout << "persantage = " << persantage << endl;
	cout << "directionXY = " << directionXY.x << " " << directionXY.y << endl;
	persantage -= 5;
	
	if (persantage <= 0){ alive = false; }
	
	for (auto spaceObject = spaceObjects.begin(); spaceObject != spaceObjects.end(); ++spaceObject){
		(*spaceObject)->change_position(directionXY);
	}
	inertia_delay = now + (std::chrono::milliseconds) INERTIA_DELAY;
}

void InertiaRotation::run(){
	auto now = std::chrono::system_clock::now();
	if (inertia_delay >= now){ return; }
	inertia_delay = now + (std::chrono::milliseconds) INERTIA_DELAY;
	
}