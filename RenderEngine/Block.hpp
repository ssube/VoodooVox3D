#pragma once

#include "Includes.hpp"
#include "BlockTemplate.hpp"

class Block :
	public BlockTemplate
{
public:
	Block(BlockTemplate * temp);
	~Block(void);

	static const size_t BlockSize = 10;
};
