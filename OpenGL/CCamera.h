#pragma once

#include "CObject.h"

class Camera : public Object {
public:
	glm::vec3 m_vf3At;
	glm::vec3 m_vf3Up;

	glm::mat4 m_mf4x4View;
	glm::mat4 m_mf4x4Proj;

	GLint m_ixViewport;
	GLint m_iyViewport;
	GLint m_iWidth;
	GLint m_iHeight;

	Camera();

	void SetView(glm::vec3 vf3Eye, glm::vec3 vf3At);
	void SetViewport(GLint ix, GLint iy, GLint iwidth, GLint iheight);

	virtual void Rotate(GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian);
	virtual void Revolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian);
	virtual void Translate(GLfloat fx, GLfloat fy, GLfloat fz);

	virtual void PrepareRender(GLuint iShaderProgramId);
};

class OrthoCamera : public Camera {
public:
	GLfloat m_fLeft;
	GLfloat m_fRight;
	GLfloat m_fBottom;
	GLfloat m_fTop;
	GLfloat m_fNear;
	GLfloat m_fFar;

	OrthoCamera();

	void SetOrtho(GLfloat fLeft, GLfloat fRight, GLfloat fBottom, GLfloat fTop, GLfloat fNear, GLfloat fFar);
};

class ProjCamera : public Camera {
public:
	GLfloat m_fFovy;
	GLfloat m_fAspect;
	GLfloat m_fNear;
	GLfloat m_fFar;

	ProjCamera();

	void SetProj(GLfloat fFovy, GLfloat fAspect, GLfloat fNear, GLfloat fFar);
};
