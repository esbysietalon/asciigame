#pragma once
#include <random>
#include "time.h"

extern std::mt19937 rng;

void initRNG(long seed=time(NULL));
