#pragma once

#include "stdafx.h"
#include "CObject.h"

class Player : public Object {
public:
	float xdir = 0, ydir = 0, zdir = 0;
	float speed = 0.1;

	bool command_w = false, command_a = false, command_s = false, command_d = false;

	Player();
	Player(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb, GLfloat fa);

	
	void Move_keydown(char direction);
	void Move_keyup(char direction);

	virtual void Update();
	virtual void Render(GLuint iShaderProgramId);

	
};
