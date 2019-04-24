#pragma once
#include "globals.h"
#include <vector>
#include "actor.h"
#include <algorithm>
#include <random>
#include "atlas.h"
#include <windows.h>
#include <string>

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
	void printLegend();

	bool isOccupied(int x, int y);

	std::uniform_real_distribution<> distr;
	double random();

	bool running = true;
	bool openPipe = false;
	bool openPipe_s = false;

	HANDLE usedPipe = NULL;
	HANDLE usedPipe_s = NULL;

	DWORD dwWritten;
	DWORD dwWritten_s;


	char input;

	std::string prefix_s = "> ";

	Atlas* world;

	std::vector<Actor*>* actors;
};