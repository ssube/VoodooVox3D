#include "BlockTemplate.hpp"

BlockTemplate::BlockTemplate(void)
{
}

BlockTemplate::~BlockTemplate(void)
{
}

/*void BlockTemplate::AddAttribute(string name,       void   * value)
{
	AttributeType type = BlockTemplate::GetType(name);
	switch ( type )
	{
	case AT_Bool:
		mBoolAttributes[name] = *(bool*)value;
		break;
	case AT_Int:
		mIntAttributes[name] = *(int*)value;
		break;
	case AT_Float:
		mFloatAttributes[name] = *(float*)value;
		break;
	case AT_String:
		mStringAttributes[name] = *(string*)value;
		break;
	}
}*/

void BlockTemplate::AddAttribute(string name,       bool   * value)
{
	mBoolAttributes[name] = *value;
}

void BlockTemplate::AddAttribute(string name,       int    * value)
{
	mIntAttributes[name] = *value;
}

void BlockTemplate::AddAttribute(string name,       float  * value)
{
	mFloatAttributes[name] = *value;
}

void BlockTemplate::AddAttribute(string name,       string * value)
{
	mStringAttributes[name] = *value;
}

void BlockTemplate::AddAttribute(string name,       char   * value)
{
	string temp = value;
	mStringAttributes[name] = temp;
}

void BlockTemplate::AddAttribute(string name, const char   * value)
{
	string temp = value;
	mStringAttributes[name] = temp;
}

void BlockTemplate::AddAttribute(string name, const bool   & value)
{
	mBoolAttributes[name] = value;
}

void BlockTemplate::AddAttribute(string name, const int    & value)
{
	mIntAttributes[name] = value;
}

void BlockTemplate::AddAttribute(string name, const float  & value)
{
	mFloatAttributes[name] = value;
}

void BlockTemplate::AddAttribute(string name, const string & value)
{
	mStringAttributes[name] = value;
}


int  BlockTemplate::GetAttribute(string name, bool   * val)
{
	map<string, bool>::iterator ittr = mBoolAttributes.find(name);
	if ( val )
	{
		if ( ittr != mBoolAttributes.end() )
		{
			*val = ittr->second;
			return sizeof(bool);
		} else {
			val = BOOL_NULL;
			return 0;
		}
	} else {
		if ( ittr != mBoolAttributes.end() )
		{
			return sizeof(bool);
		} else {
			return 0;
		}
	}
}

int  BlockTemplate::GetAttribute(string name, int    * val)
{
	map<string, int>::iterator ittr = mIntAttributes.find(name);
	if ( val )
	{
		if ( ittr != mIntAttributes.end() )
		{
			*val = ittr->second;
			return sizeof(int);
		} else {
			val = INT_NULL;
			return 0;
		}
	} else {
		if ( ittr != mIntAttributes.end() )
		{
			return sizeof(int);
		} else {
			return 0;
		}
	}
}

int  BlockTemplate::GetAttribute(string name, float  * val)
{
	map<string, float>::iterator ittr = mFloatAttributes.find(name);
	if ( val )
	{
		if ( ittr != mFloatAttributes.end() )
		{
			*val = ittr->second;
			return sizeof(float);
		} else {
			val = FLOAT_NULL;
			return 0;
		}
	} else {
		if ( ittr != mFloatAttributes.end() )
		{
			return sizeof(float);
		} else {
			return 0;
		}
	}
}

int  BlockTemplate::GetAttribute(string name, string * val)
{
	map<string, string>::iterator ittr = mStringAttributes.find(name);
	if ( val )
	{
		if ( ittr != mStringAttributes.end() )
		{
			*val = ittr->second;
			return ittr->second.length();
		} else {
			val = STRING_NULL;
			return false;
		}
	} else {
		if ( ittr != mStringAttributes.end() )
		{
			return ittr->second.length();
		} else {
			return 0;
		}
	}
}

AttributeType BlockTemplate::GetType(string name)
{
	char indicator = name[0];
	switch ( indicator )
	{
	case 'b':
		return AT_Bool;
	case 'f':
		return AT_Float;
	case 'i':
		return AT_Int;
	case 's':
		return AT_String;
	default:
		return AT_Unknown;
	}
}
/*
BlockTemplate::BlockTemplate(void)
{
}

BlockTemplate::~BlockTemplate(void)
{
}
*/
