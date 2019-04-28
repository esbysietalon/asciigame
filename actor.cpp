#include "stdafx.h"
#include "actor.h"

Actor::Actor(int t, int tt)
{
	x = t;
	y = tt;
	goal_x = t;
	goal_y = tt;
	ai = aistate_t::READY;
	label = 'X';//0xDB;
}

void Actor::assume()
{
	ai = aistate_t::PLAYER;
}

char Actor::getAIState()
{
	return ai;
}

void Actor::setAIState(char state)
{
	ai = state;
}

int Actor::getX()
{
	return x;
}
int Actor::getY()
{
	return y;
}

int Actor::getGoalX()
{
	return goal_x;
}
int Actor::getGoalY()
{
	return goal_y;
}
void Actor::setX(int num)
{
	x = num;
}

void Actor::setY(int num)
{
	y = num;
}

void Actor::setGoal(int x, int y)
{
	goal_x = x;
	goal_y = y;
}

char Actor::getLabel()
{
	return label;
}

int Actor::getIndex()
{
	return x + y * MAP_WIDTH;
}

void Actor::setPath(char ** newPath)
{
	path = newPath;
}

char ** Actor::getPath()
{
	return path;
}
