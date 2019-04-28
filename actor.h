#pragma once
#include "globals.h"

class Actor {
public:
	Actor(int t, int tt);
	~Actor();

	void assume();
	char getAIState();
	void setAIState(char state);
	int getX();
	int getY();
	void setX(int num);
	void setY(int num);
	char getLabel();
	int getIndex();
	void setPath(char** newPath);
	char** getPath();
private:
	char** path;
	int x, y;
	char ai;
	char label;
};