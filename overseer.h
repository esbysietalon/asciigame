#pragma once
#include "globals.h"
#include <vector>
#include "actor.h"
#include <algorithm>
#include <random>
#include "atlas.h"



class Atlas;
class Overseer {
public:
	Overseer();
	~Overseer();
private:
	void init();
	void run();
	void print();
	void enterActor(Actor* actor);
	void populate();
	void update();

	bool isOccupied(int x, int y);

	std::uniform_real_distribution<> distr;
	double random();

	bool running = true;

	Atlas* world;

	std::vector<Actor*>* actors;
};