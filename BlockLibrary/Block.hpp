#pragma once

#include "Includes.hpp"
#include "BlockTemplate.hpp"

class __declspec(dllexport) Block :
	public BlockTemplate
{
public:
	Block(BlockTemplate * temp);
	~Block(void);

	int Health;
	float Light;
};
