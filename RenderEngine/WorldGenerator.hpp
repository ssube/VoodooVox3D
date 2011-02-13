#pragma once

#include "Includes.hpp"

class __declspec(dllexport) Generator
{
public:
	Generator(int seed);
	~Generator();

	float GetPoint(float x, float y, float z);

private:
	int mValues[WORLD_BLOCKS][WORLD_BLOCKS][WORLD_BLOCKS];

	float Interp(float a, float b, float x);
	float ToFloat(int i);
};
