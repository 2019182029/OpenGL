#include "CPlayer.h"

Player::Player() {
	m_pMesh = new AirplaneMesh();
	m_sType = "Player";
}

Player::Player(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb, GLfloat fa) : Object(fx, fy, fz, flength) {
	m_pMesh = new AirplaneMesh(flength, fr, fg, fb, fa);
	if (fa != 1.0f) { m_bTranslucent = true; }
	m_sType = "Player";
}
void Player::Render(GLuint iShaderProgramId) {
	Object::PrepareRender(iShaderProgramId);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_pMesh->m_vVertices.size());
}
void Player::Update() {
	if (m_vf3Position.x + xdir < -2.0f + m_flength || m_vf3Position.x + xdir > 2.0f - m_flength) xdir = 0;
	if (m_vf3Position.y + ydir < -2.0f + m_flength || m_vf3Position.y + ydir > 2.0f - m_flength) ydir = 0;

	Translate(xdir, ydir, zdir);

	Object::Update();
}

void Player::Move_keydown(char direction) {
	switch (direction) {
	case 'w':			//╩С
		xdir = 0.0f;
		ydir = 0.05f;
		zdir = 0.0f;
		break;
	case 'a':			//аб
		xdir = -0.05f;
		ydir = 0.0f;
		zdir = 0.0f;
		break;
	case 's':			//го
		xdir = 0.0f;
		ydir = -0.05f;
		zdir = 0.0f;
		break;
	case 'd':			//©Л
		xdir = 0.05f;
		ydir = 0.0f;
		zdir = 0.0f;
		break;

	default:
		break;
	}
}

void Player::Move_keyup(char direction) {
	switch (direction) {
	case 'w':
		xdir = 0.0f;
		ydir = 0.0f;
		zdir = 0.0f;
	case 'a':
		xdir = 0.0f;
		ydir = 0.0f;
		zdir = 0.0f;
	case 's':
		xdir = 0.0f;
		ydir = 0.0f;
		zdir = 0.0f;
	case 'd':
		xdir = 0.0f;
		ydir = 0.0f;
		zdir = 0.0f;
		break;

	default:
		break;
	}
}