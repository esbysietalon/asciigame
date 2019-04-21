#include "stdafx.h"
#include "map.h"

Map::Map(int w, int h)
{
	map = new char[w * h];
	width = w;
	height = h;
}

Map::~Map()
{
}

char ** Map::get()
{
	return &map;
}

int Map::getWidth()
{
	return width;
}

int Map::getHeight()
{
	return height;
}
