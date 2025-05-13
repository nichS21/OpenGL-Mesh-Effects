#include "utils.h"
#include <cmath>

std::default_random_engine gen(time(0));                    //random engine, seeded with current time
std::uniform_real_distribution<float> unif(0.0, 1.0);		//for generating random numbers in range 0-1



bool eq_zero(float value)
{
    return abs(value) <= ZERO;
}

bool gt_zero(float value)
{
    return value > ZERO;
}

float genFloat()
{
    return unif(gen);
}