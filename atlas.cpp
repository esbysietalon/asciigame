#include "stdafx.h"
#include "atlas.h"
#include "globals.h"

Atlas::Atlas()
{
	maps = new std::vector<Map*>();
	generator = new MapGen();
}

Atlas::~Atlas()
{
}

char Atlas::getAt(int x, int y)
{
	if(x + y * maps->at(page)->getWidth() < 0 || x + y * maps->at(page)->getWidth() >= maps->at(page)->getWidth() * maps->at(page)->getHeight()){
		return ' ';
	}
	return (*maps->at(page)->get())[x + y * maps->at(page)->getWidth()];
}

void Atlas::create(int w, int h) {
	Map* map = new Map(w, h);
	generate(map, map_t::HALLWAYS);
	maps->push_back(map);
}

int Atlas::turnTo(int num)
{
	if (num > maps->size() || num < 0) {
		return -1;
	}else{
		page = num;
		return 0;
	}
}

void Atlas::generate(Map* map, map_t type) {
	generator->generate(map->get(), map->getWidth(), map->getHeight(), type);
}
