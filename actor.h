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
	int getGoalX();
	int getGoalY();
	void setX(int num);
	void setY(int num);
	void setGoal(int x, int y);
	char getLabel();
	int getIndex();
	void setPath(char** newPath);
	char** getPath();
private:
	char** path;
	int x, y;
	int goal_x, goal_y;
	char ai;
	char label;
};