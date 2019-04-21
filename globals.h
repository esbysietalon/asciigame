#pragma once

#define MAP_HEIGHT 50
#define MAP_WIDTH 150
#define ACTOR_NUM 0
#define HALLSNUM 15
#define ROOMSNUM 2
#define MINROOMLEN 3
#define MAXROOMLEN 25

enum terrain_t {EMPTY='.', WALL='+'};
enum map_t {RANDOM, HALLWAYS, ROOMS};