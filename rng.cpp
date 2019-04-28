#include "stdafx.h"
#include "rng.h"
std::mt19937 rng;
void initRNG(long seed)
{
	rng.seed(seed);
}
