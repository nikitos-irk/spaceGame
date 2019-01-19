#ifndef INERTIA_H
#define INERTIA_H

#include "SpaceShip.hpp"
#include <cmath>

#define INERTIA_DELAY 10

class Inertia{
protected:
	bool alive;
	int persantage;
	long direction_v;
	DirectionXY directionXY;
	std::chrono::time_point<std::chrono::system_clock> inertia_delay;
	std::chrono::time_point<std::chrono::system_clock> inertia_duration;
public:
	Inertia(SpaceShip*, vector<SpaceObject*> &);
	vector<SpaceObject*>& spaceObjects;
	bool isAlive();
	virtual void run();
	virtual ~Inertia();
};

class InertiaUpDown: public Inertia{
private:
	bool forwardDirection;
public:
	InertiaUpDown(SpaceShip*, vector<SpaceObject*> &, bool);
	~InertiaUpDown();
	void run();
};

class InertiaRotation: public Inertia{
private:
	bool clockwiseDirection;
public:
	InertiaRotation(SpaceShip*, vector<SpaceObject*> &, bool);
	~InertiaRotation();
	void run();
};
#endif