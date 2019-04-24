#pragma once
#include "globals.h"
class Actor {
public:
	Actor(int t, int tt);
	~Actor();

	void assume();
	bool getAIState();
	int getX();
	int getY();
	void setX(int num);
	void setY(int num);
	char getLabel();
	int getIndex();
private:
	int x, y;
	bool ai;
	char label;
};