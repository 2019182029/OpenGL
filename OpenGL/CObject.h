#pragma once

#include "stdafx.h"
#include "CMesh.h"

class Object {
public:
	GLuint vao, vbo, ebo;

	glm::mat4 m_mf4x4World;

	glm::vec3 m_vf3Position;
	glm::vec3 m_vf3RotationSpeed;
	glm::vec3 m_vf3RevolutionPosition;
	glm::vec3 m_vf3RevolutionSpeed;
	glm::vec3 m_vf3ScaleRatio;

	GLfloat m_flength;

	Mesh* m_pMesh;
	glm::vec3 m_AABB[2];

	std::string m_sType;

	Object();
	Object(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength);
	~Object();

	void SetVbo();
	void SetRotationSpeed(GLfloat fx, GLfloat fy, GLfloat fz);
	void SetRevolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian);

	virtual void Rotate(GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian);
	virtual void Revolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian);
	virtual void Translate(GLfloat fx, GLfloat fy, GLfloat fz);
	void Scale(GLfloat fx, GLfloat fy, GLfloat fz);
	virtual void Update();
	void UpdateBoundingBox();
	virtual void PrepareRender(GLuint iShaderProgramId);
	virtual void Render(GLuint iShaderProgramId);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TriangleObject : public Object {
public:
	TriangleObject();
	TriangleObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

class SquareObject : public Object {
public:
	SquareObject();
	SquareObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CubeObject : public Object {
public:
	CubeObject();
	CubeObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

class SphereObject : public Object {
public:
	SphereObject();
	SphereObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

class PyramidObject : public Object {
public:
	PyramidObject();
	PyramidObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Obstacle : public Object {
public:
	bool m_bIsBlowingUp;
	float m_fElapsedTime;

	Mesh* m_pExplosionMesh;
	glm::vec3 m_pvf3Vectors[64];
	glm::mat4 m_pmf4x4Transforms[64];

	Obstacle();
	Obstacle(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);

	virtual void Update();
	void PrepareExplosion();
	virtual void Render(GLuint iShaderProgramId);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class HierarchyObject : public Object {
public:
	GLUquadric* m_pQobj;

	glm::vec3 m_vf3RotationOffset;
	glm::vec3 m_vf3RevolutionOffset;

	glm::mat4 m_mf4x4Transform;
	std::string m_sFrameName;
	Object* m_pChildObject;
	Object* m_pSiblingObject;

	HierarchyObject();
	HierarchyObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);

	void SetFrameName(std::string sFrameName);
	void SetChildObject(Object* pChildObject);
	void SetSiblingObject(Object* pSiblingObject);

	virtual void Rotate(GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian);
	virtual void Revolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian);
	virtual void Update();
	void UpdateTransform(glm::mat4* pmf4x4Parent);
	virtual void Render(GLuint iShaderProgramId);

	Object* FindFrame(std::string sFrameName);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Qobject : public Object {
public:
	GLUquadric* m_pQobj;

	Qobject();
	Qobject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Axis : public Object {
public:
	Axis();

	virtual void Render(GLuint iShaderProgramId);
};
