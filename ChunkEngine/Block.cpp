#include "Block.hpp"

Block::Block(BlockTemplate * temp)
	: BlockTemplate(*temp)
{
	//mBoolAttributes   = temp->mBoolAttributes  ;
	//mFloatAttributes  = temp->mFloatAttributes ;
	//mIntAttributes    = temp->mIntAttributes   ;
	//mStringAttributes = temp->mStringAttributes;
}

Block::~Block(void)
{
}
