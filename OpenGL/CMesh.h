#pragma once

#include "stdafx.h"

class Vertex {
public:
	GLfloat x;  GLfloat y;  GLfloat z;
	glm::vec3 normal;

	Vertex() { x = 0.0f; y = 0.0f; z = 0.0f; };
	Vertex(GLfloat fx, GLfloat fy, GLfloat fz) : x(fx), y(fy), z(fz) {};
};

class DiffusedVertex : public Vertex {
public:
	GLfloat r;  GLfloat g;  GLfloat b;

	DiffusedVertex() : Vertex() { r = 0.0f; g = 0.0f; b = 0.0f; };
	DiffusedVertex(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fr = 0.0f, GLfloat fg = 0.0f, GLfloat fb = 0.0f) : Vertex(fx, fy, fz), r(fr), g(fg), b(fb) {};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Mesh {
public:
	GLfloat m_flength;

	std::vector<DiffusedVertex> m_vVertices;
	std::vector<unsigned int> m_uiIndicies;

	Mesh();
	Mesh(GLfloat flength);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TriangleMesh : public Mesh {
public:
	TriangleMesh();
	TriangleMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

class SquareMesh : public Mesh {
public:
	SquareMesh();
	SquareMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CubeMesh : public Mesh {
public:
	CubeMesh();
	CubeMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

class SphereMesh : public Mesh {
public:
	SphereMesh();
	SphereMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

class PyramidMesh : public Mesh {
public:
	PyramidMesh();
	PyramidMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AirplaneMesh : public Mesh {
public:
	AirplaneMesh();
	AirplaneMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AxisMesh : public Mesh {
public:
	AxisMesh();
};
