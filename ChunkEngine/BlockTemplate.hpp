#pragma once

#include "Includes.hpp"

using namespace std;

enum __declspec(dllexport) AttributeType
{
	AT_Unknown = 0,
	//---------
	AT_Bool,
	AT_Float,
	AT_Int,
	AT_String,
	//---------
	AT_Max
};

class __declspec(dllexport) BlockTemplate
{
public:
	BlockTemplate(void)
	{
		mAttributes.clear();
	}

	BlockTemplate(const BlockTemplate & other)
	{
		// Copy each attribute
	}

	~BlockTemplate(void)
	{
		map<string, void*>::iterator ittr = mAttributes.begin();
		while ( ittr != mAttributes.end() )
		{
			free(ittr->second);
		}
		mAttributes.clear();
	}

	template<typename T>
	void AddAttribute(string name, T * value)
	{
		AttributeType type = GetType(name);

		void * val = NULL;

		if ( type == AT_Bool )
		{
			val = malloc(sizeof(bool));
			memcpy(val, value, sizeof(bool));
		} else if ( type == AT_Float ) {
			val = malloc(sizeof(float));
			memcpy(val, value, sizeof(float));
		} else if ( type == AT_Int ) {
			val = malloc(sizeof(int));
			memcpy(val, value, sizeof(int));
		} else if ( type == AT_String ) {
			val = malloc(32);
			memcpy(val, value, 32);
			((char*)val)[31] = 0x00;
		}

		mAttributes[name] = val;
	}

	template<typename T>
	void AddAttribute(string name, const T & value)
	{
		AttributeType type = GetType(name);

		void * val = NULL;

		if ( type == AT_Bool )
		{
			val = malloc(sizeof(bool));
			memcpy(val, &value, sizeof(bool));
		} else if ( type == AT_Float ) {
			val = malloc(sizeof(float));
			memcpy(val, &value, sizeof(float));
		} else if ( type == AT_Int ) {
			val = malloc(sizeof(int));
			memcpy(val, &value, sizeof(int));
		} else if ( type == AT_String ) {
			val = malloc(32);
			memcpy(val, &value, 32);
			((char*)val)[31] = 0x00;
		}

		mAttributes[name] = val;
	}

	template<typename T>
	T * GetAttribute(string name)
	{
		map<string,void*>::iterator ittr = mAttributes.find(name);
		if ( ittr != mAttributes.end() )
		{
			return (T*)ittr->second;
		} else {
			return NULL;
		}
	}

	static AttributeType GetType(string name)
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

private:
	map<string, void*> mAttributes;
};
