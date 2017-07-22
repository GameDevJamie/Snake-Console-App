#pragma once

#include <math.h>
#include <windows.h>

inline int Random(const int a, const int b)
{
	int t = (b - a + 1) * rand();
	return t == 0 ? a : a + (t - 1) / RAND_MAX;
}