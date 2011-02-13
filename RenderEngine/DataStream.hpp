#pragma once

#include "Includes.hpp"

class DataStream
{
public:
	void * GetBuffer()
	{
		return mBytes.begin()._Myptr;
	}

	size_t GetSize()
	{
		return mBytes.size();
	}

	void SetBuffer(size_t size, void * data)
	{
		mBytes.clear();
		for ( size_t i = 0; i < size; ++i )
		{
			mBytes.push_back(((unsigned char*)data)[i]);
		}
	}

	template<typename T>
	void Write(T obj)
	{
		size_t newBytes = sizeof(obj);
		for ( size_t i = 0; i < newBytes; ++i )
		{
			mBytes.push_back(i);
		}
	}

private:
	std::vector<unsigned char> mBytes;
};
