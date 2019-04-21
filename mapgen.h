#pragma once
#include "globals.h"
#include <random>





class MapGen {
public:
	MapGen();
	~MapGen();

	void generate(char** map, int w, int h, int type = 0);
private:
	std::uniform_real_distribution<> distr;
	double random();

	void placeRooms(char** map);
	void recurseHallways(char** map, int x, int y, int dir, int level);
};