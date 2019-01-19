#include "Inertia.hpp"

Inertia::Inertia(SpaceShip* spaceShip, vector<SpaceObject*> &spaceObjects): spaceObjects(spaceObjects), directionXY(0, 0){
	this->spaceObjects = spaceObjects;
	this->directionXY = spaceShip->get_direction();
	this->alive = true;
	this->persantage = 100;
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
	if (inertia_delay >= now){ return; }

	if (abs(directionXY.x) <= 0 && abs(directionXY.y) <= 0 ){
		cout << "directionXY = " << directionXY.x << " " << directionXY.y << endl;
		return;
	}

	if (!forwardDirection){
		directionXY.x *= -1;
		directionXY.y *= -1;
	}
	
	directionXY.x -= pow(directionXY.x, 1/4);
	directionXY.y -= pow(directionXY.y, 1/4);
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