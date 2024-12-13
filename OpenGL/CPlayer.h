#pragma once

#include "stdafx.h"
#include "CObject.h"

class Player : public Object {
public:
	float xdir = 0.0f, ydir = 0.0f, zdir = 0.0f;
	float speed = 0.1f;

	bool command_w = false, command_a = false, command_s = false, command_d = false;

	bool m_bIsBlowingUp;
	float m_fElapsedTime;

	Mesh* m_pExplosionMesh;
	glm::vec3 m_pvf3Vectors[64];
	glm::mat4 m_pmf4x4Transforms[64];

	int m_iHP;
	int number_of_bullets;

	Player();
	Player(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb, GLfloat fa);
	
	void Move_keydown(char direction);
	void Move_keyup(char direction);

	virtual void Update();
	void PrepareExplosion();
	virtual void Render(GLuint iShaderProgramId);
};

class Bullet : public Object {
public:
	float xdir = 0.0f, ydir = 0.0f, zdir = 0.0f;
	float speed = 0.1f;

	Bullet();
	Bullet(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb, GLfloat fa);

	virtual void Update();
};

