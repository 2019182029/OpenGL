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
	if (m_vf3Position.x + xdir < -2.0f + m_flength || m_vf3Position.x + xdir > 2.0f - m_flength) {
		if (command_a) { xdir += 0.05f; command_a = false; }
		if (command_d) { xdir += -0.05f; command_d = false; }

	}

	if (m_vf3Position.y + ydir < -2.0f + m_flength || m_vf3Position.y + ydir > 2.0f - m_flength) {
		if (command_w) { ydir += -0.05f; command_w = false; }
		if (command_s) { ydir += 0.05f; command_s = false; }
	}
	Translate(xdir, ydir, zdir);
	Object::Update();
}

void Player::Move_keydown(char direction) {
	switch (direction) {
	case 'w':			//╩С
		if (command_w == false) {

			ydir += 0.05f;
			command_w = true;
		}
		break;
	case 'a':			//аб
		if (command_a == false) {
			xdir += -0.05f;
			command_a = true;
		}
		break;
	case 's':			//го
		if (command_s == false) {
			ydir += -0.05f;
			command_s = true;
		}
		break;
	case 'd':			//©Л
		if (command_d == false) {
			xdir += 0.05f;
			command_d = true;
		}
		break;

	default:
		break;
	}
}

void Player::Move_keyup(char direction) {
	switch (direction) {
	case 'w':
		if (command_w) {
			ydir += -0.05f;
			command_w = false;
		}
		break;
	case 'a':
		if (command_a) {
			xdir += 0.05f;
			command_a = false;
		}
		break;
	case 's':
		if (command_s) {
			ydir += 0.05f;
			command_s = false;
		}
		break;
	case 'd':
		if (command_d) {
			xdir += -0.05f;
			command_d = false;
		}
		break;

	default:
		break;
	}


}