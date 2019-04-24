#include "stdafx.h"
#include "actor.h"

Actor::Actor(int t, int tt)
{
	x = t;
	y = tt;
	ai = true;
	label = 'X';//0xDB;
}

void Actor::assume()
{
	ai = false;
}

bool Actor::getAIState()
{
	return ai;
}

int Actor::getX()
{
	return x;
}
int Actor::getY()
{
	return y;
}

void Actor::setX(int num)
{
	x = num;
}

void Actor::setY(int num)
{
	y = num;
}

char Actor::getLabel()
{
	return label;
}

int Actor::getIndex()
{
	return x + y * MAP_WIDTH;
}
