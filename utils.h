#ifndef UTILS_H
#define UTILS_H

#include <numbers>
#include <random>

const float ZERO = .1;
const float PI = std::numbers::pi_v<float>;			//PI for use in EQs in all shapes


bool eq_zero(float value);
bool gt_zero(float value);
float genFloat();									//generate random float in range 0 - 1

#endif