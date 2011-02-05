#pragma once

#include "Includes.hpp"
#include "VertexHeader.hpp"

struct Vertex
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT tu, tv, tw;
	DWORD color;

	Vertex()
		: x(0), y(0), z(0), nx(0), ny(0), nz(0), tu(0), tv(0), tw(0), color(0xFFFF0000)
	{ };

	Vertex(float x, float y, float z, float nx, float ny, float nz, float tu, float tv, float tw, DWORD color)
		: x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), tu(tu), tv(tv), tw(tw), color(color)
	{ };
};
