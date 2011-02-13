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
	BlockTemplate(void);
	~BlockTemplate(void);

	void AddAttribute(string name,       bool   * value);
	void AddAttribute(string name,       int    * value);
	void AddAttribute(string name,       float  * value);
	void AddAttribute(string name,       string * value);
	void AddAttribute(string name,       char   * value);
	void AddAttribute(string name, const char   * value);

	void AddAttribute(string name, const bool   & value);
	void AddAttribute(string name, const int    & value);
	void AddAttribute(string name, const float  & value);
	void AddAttribute(string name, const string & value);

	int  GetAttribute(string name,       bool   * value);
	int  GetAttribute(string name,       int    * value);
	int  GetAttribute(string name,       float  * value);
	int  GetAttribute(string name,       string * value);

	static AttributeType GetType(string name);

protected:
	map<string, bool>   mBoolAttributes;
	map<string, int>    mIntAttributes;
	map<string, float>  mFloatAttributes;
	map<string, string> mStringAttributes;
};
