#include "stdafx.h"
#include "mapgen.h"
#include "rng.h"
#include <iostream>
MapGen::MapGen()
{

}

double MapGen::random() {
	double out = distr(rng);
	//std::cout << out << std::endl;
	return out;
}


void MapGen::recurseHallways(char** map, int x, int y, int dir, int level) {
	int currLen = 0;
	std::uniform_int_distribution<> randd(0, 3);
	if (level > 3) {
		return;
	}
	while (random() > 0.01 * currLen && x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
		(*map)[x + y * MAP_WIDTH] = terrain_t::EMPTY;
		currLen++;
		switch (dir) {
		case 0:
			y++;
			break;
		case 1:
			x++;
			break;
		case 2:
			y--;
			break;
		case 3:
			x--;
		}
		if (random() > 0.85) {
			int newDir = randd(rng);
			while (newDir % 2 == dir % 2) {
				newDir = randd(rng);
			}
			recurseHallways(map, x, y, newDir, level+1);
		}
	}
}
void MapGen::generate(char ** map, int w, int h, int type)
{
	std::uniform_int_distribution<> randw(0, MAP_WIDTH);
	std::uniform_int_distribution<> randh(0, MAP_HEIGHT);
	std::uniform_int_distribution<> randd(0, 3);
	switch (type) {
	case 0:
		for (int i = 0; i < w*h; i++) 
			(*map)[i] = (random() > 0.5 ? terrain_t::EMPTY : terrain_t::WALL);
		break;
	case 1:
		for (int i = 0; i < w*h; i++)
			(*map)[i] = terrain_t::WALL;
		for (int i = 0; i < HALLSNUM; i++) {
			recurseHallways(map, randw(rng), randh(rng), randd(rng), 0);
			recurseHallways(map, randw(rng), randh(rng), randd(rng), 0);
			recurseHallways(map, randw(rng), randh(rng), randd(rng), 0);
		}
		break;
	case 2:
		break;
	}
	
}
