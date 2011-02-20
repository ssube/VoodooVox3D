#pragma once

#include "Includes.hpp"

using namespace std;

struct WORLDMANAGER_API BlockTemplate
{
public:
	unsigned char ID;
	unsigned char DefaultHealth;
	char Name[16];
	float Speed;
	size_t Texture;
	bool Occludes;
	bool Visible;
};
