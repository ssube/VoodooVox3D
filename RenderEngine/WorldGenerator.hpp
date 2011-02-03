#pragma once

#include <stdlib.h>
#include <math.h> 

class __declspec(dllexport) Generator
{
public:
	Generator(int seed);
	~Generator();

	float GetPoint(float x, float y, float z);

private:
	int * mValues;

	float Interp(float a, float b, float x);
	float ToFloat(int i);
};
