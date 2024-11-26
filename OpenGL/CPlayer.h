#pragma once

#include "stdafx.h"
#include "CObject.h"

class Player : public Object {
public:
	Player();
	Player(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb);

	virtual void Render(GLuint iShaderProgramId);
};
