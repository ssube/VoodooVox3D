#pragma once

#include "Includes.hpp"

using namespace std;

struct __declspec(dllexport) BlockTemplate
{
public:
	int ID;
	char Name[16];
	float Speed;
	size_t Texture;
	bool Occludes;
	bool Visible;
	int DefaultHealth;
};
