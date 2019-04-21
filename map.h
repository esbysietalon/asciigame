#pragma once
#include <iostream>
#include "mapgen.h"
class Map {
public:
	Map(int w, int h);
	~Map();
	char** get();
	int getWidth();
	int getHeight();
private:
	char* map;	
	int width, height;

};