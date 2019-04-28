#pragma once
#include "globals.h"
#include <vector>
#include "actor.h"
#include <algorithm>
#include <random>
#include "atlas.h"
#include <windows.h>
#include <string>


struct intpair_t {
	int x, y;
	char move;
	char origin;
	int px, py;
	float cost;
	float scost;
	intpair_t(int i, int j) {
		x = i;
		y = j;
		move = ' ';
		origin = ' ';
		px = -1;
		py = -1;
		cost = 1;
	}
	intpair_t() {
		x = -1;
		y = -1;
		move = ' ';
		origin = ' ';
		px = -1;
		py = -1;
		cost = 1;
	}
};
class Atlas;
class Overseer {
public:
	Overseer(HANDLE pipe, HANDLE pipe2);
	~Overseer();

	void report(char* message, int msglen);
	void tell(char* message, int msglen);
	void report(const char* message, int msglen);
	void tell(const char* message, int msglen);
private:
	void init();
	void run();
	void print();
	void enterActor(Actor* actor);
	void populate();
	void update();
	void parseInput(int index, std::string* std_tell);
	void parseInput(int index, std::string* std_tell, char in);
	void think(int index, std::string* std_tell);
	
	bool dijkstraesque(int index, int sx, int sy, int ex, int ey);

	DWORD getInput(INPUT_RECORD** eventBuffer);

	bool isOccupied(int x, int y, int actor=-1);

	std::uniform_real_distribution<> distr;
	double random();

	bool running = true;
	bool openPipe = false;
	bool openPipe_s = false;

	HANDLE usedPipe = NULL;
	HANDLE usedPipe_s = NULL;

	HANDLE wHnd, rHnd;
	COORD characterBufferSize, bufferSize;
	COORD characterPosition;
	SMALL_RECT consoleWriteArea;
	SMALL_RECT windowSize;

	INPUT_RECORD* eventBuffer;
	
	DWORD dwWritten;
	DWORD dwWritten_s;

	char* legend = "Press x to exit. Press 'qweasdzc' to navigate.\n";
	//std::vector<char> path;

	std::vector<std::vector<char>> paths;

	char input;

	CHAR_INFO* consoleBuffer;

	std::string prefix_s = "> ";

	Atlas* world;

	std::vector<Actor*>* actors;
};