#include "Block.hpp"

Block::Block(BlockTemplate * temp)
	: BlockTemplate(*temp), Health(temp->DefaultHealth), Light(1.0f)
{
	//mBoolAttributes   = temp->mBoolAttributes  ;
	//mFloatAttributes  = temp->mFloatAttributes ;
	//mIntAttributes    = temp->mIntAttributes   ;
	//mStringAttributes = temp->mStringAttributes;
}

Block::~Block(void)
{
}
