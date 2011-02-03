#pragma once

#include "Includes.hpp"
#include "BlockTemplate.hpp"

class Block :
	public BlockTemplate
{
public:
	Block(BlockTemplate * temp);
	~Block(void);
};
