#pragma once

#define WINDOW_WIDTH 990
#define WINDOW_HEIGHT 900
#define MAP_HEIGHT 100
#define MAP_WIDTH 100
#define ACTOR_NUM 10
#define HALLSNUM 15
#define ROOMSNUM 7
#define MINROOMLEN 3
#define MAXROOMLEN 25
#define BGCOLOR BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN
#define PLAYERCOLOR BGCOLOR | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define AICOLOR BGCOLOR | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY
#define DEFAULTCOLOR BGCOLOR
enum terrain_t {EMPTY=' ', WALL=0xB1};
enum map_t {RANDOM, HALLWAYS, ROOMS};