#include "stdafx.h"
#include "mapgen.h"
#include "rng.h"
#include <iostream>
#include <array>
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
		if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1)
			break;
		(*map)[x + y * MAP_WIDTH] = random() > MUDCOEFF ? terrain_t::EMPTY : terrain_t::MUD;
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

struct Room_t {
	
	int x, y, w, h;
	Room_t(int a, int b, int c, int d) {
		x = a;
		y = b;
		w = c;
		h = d;
	}
};

void MapGen::placeRooms(char** map) {
	std::uniform_int_distribution<> randx(0, MAP_WIDTH);
	std::uniform_int_distribution<> randy(0, MAP_HEIGHT);
	std::uniform_int_distribution<> randr(MINROOMLEN, MAXROOMLEN);
	int x, y, w, h;
	std::array<Room_t*, ROOMSNUM>* rooms = new std::array<Room_t*, ROOMSNUM>();
	for (int i = 0; i < ROOMSNUM; i++) {
		w = randr(rng);
		h = randr(rng);
		x = randx(rng);
		y = randy(rng);
		if (x < 1) {
			x = 1;
		}
		if (x > MAP_WIDTH - w - 2) {
			x = MAP_WIDTH - w - 2;
		}
		if (y < 1) {
			y = 1;
		}
		if (y > MAP_HEIGHT - h - 2) {
			y = MAP_HEIGHT - h - 2;
		}
		(*rooms)[i] = new Room_t(x, y, w, h);
		for (int iy = y; iy < y + h; iy++) {
			for (int ix = x; ix < x + w; ix++) {
				(*map)[ix + iy * MAP_WIDTH] = random() > MUDCOEFF ? terrain_t::EMPTY : terrain_t::MUD;
			}
		}
	}
	std::shuffle(rooms->begin(), rooms->end(), rng);
	for (int i = 0; i < ROOMSNUM; i++) {
		int sx = (*rooms)[i % ROOMSNUM]->x + (*rooms)[i % ROOMSNUM]->w / 2;
		int sy = (*rooms)[i % ROOMSNUM]->y + (*rooms)[i % ROOMSNUM]->h / 2;
		int ex = (*rooms)[(i + 1) % ROOMSNUM]->x + (*rooms)[(i + 1) % ROOMSNUM]->w / 2;
		int ey = (*rooms)[(i + 1) % ROOMSNUM]->y + (*rooms)[(i + 1) % ROOMSNUM]->h / 2;
		if (random() > 0.5) {
			int j = sx;
			while (j != ex) {
				(*map)[j + sy * MAP_WIDTH] = random() > MUDCOEFF ? terrain_t::EMPTY : terrain_t::MUD;
				j += (ex - sx) / abs(ex - sx);
				if (random() > 0.9) {
					recurseHallways(map, j, sy, (random() > 0.5 ? 0 : 2), 0);
				}
			}
			j = sy;
			while (j != ey) {
				(*map)[ex + j * MAP_WIDTH] = random() > MUDCOEFF ? terrain_t::EMPTY : terrain_t::MUD;
				j += (ey - sy) / abs(ey - sy);
				if (random() > 0.9) {
					recurseHallways(map, ex, j, (random() > 0.5 ? 1 : 3), 0);
				}
			}
		}
		else {
			int j = sy;
			while (j != ey) {
				(*map)[sx + j * MAP_WIDTH] = random() > MUDCOEFF ? terrain_t::EMPTY : terrain_t::MUD;
				j += (ey - sy) / abs(ey - sy);
				if (random() > 0.9) {
					recurseHallways(map, sx, j, (random() > 0.5 ? 1 : 3), 0);
				}
			}
			j = sx;
			while (j != ex) {
				(*map)[j + ey * MAP_WIDTH] = random() > MUDCOEFF ? terrain_t::EMPTY : terrain_t::MUD;
				j += (ex - sx) / abs(ex - sx);
				if (random() > 0.9) {
					recurseHallways(map, j, ey, (random() > 0.5 ? 0 : 2), 0);
				}
			}
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
		}
		break;
	case 2:
		for (int i = 0; i < w*h; i++)
			(*map)[i] = terrain_t::WALL;
		placeRooms(map);
		break;
	}
	
}
