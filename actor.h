#pragma once
#include "globals.h"
class Actor {
public:
	Actor(int t, int tt);
	~Actor();

	void assume();
	int getX();
	int getY();
	char getLabel();
	int getIndex();
private:
	int x, y;
	bool ai;
	char label;
};