#pragma once
#include "mapgen.h"
#include "map.h"
#include <vector>
enum map_t;
class Atlas {
public:
	Atlas();
	~Atlas();

	char getAt(int x, int y);
	void create(int w, int h);
	int turnTo(int num);
private:
	void generate(Map* arr, map_t type = map_t::RANDOM);
	
	std::vector<Map*>* maps;

	MapGen* generator;

	int page = 0;
};