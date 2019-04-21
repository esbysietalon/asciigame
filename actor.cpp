#include "stdafx.h"
#include "actor.h"

Actor::Actor(int t, int tt)
{
	x = t;
	y = tt;
	ai = true;
	label = '@';
}

void Actor::assume()
{
	ai = false;
}

int Actor::getX()
{
	return x;
}
int Actor::getY()
{
	return y;
}

char Actor::getLabel()
{
	return label;
}

int Actor::getIndex()
{
	return x + y * MAP_WIDTH;
}
