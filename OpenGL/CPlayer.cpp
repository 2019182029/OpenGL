#include "CPlayer.h"

Player::Player() {
	m_pMesh = new AirplaneMesh();
	m_sType = "Player";
}

Player::Player(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Object(fx, fy, fz, flength) {
	m_pMesh = new AirplaneMesh(flength, fr, fg, fb);
	m_sType = "Player";
}
void Player::Render(GLuint iShaderProgramId) {
	Object::PrepareRender(iShaderProgramId);
	
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_pMesh->m_vVertices.size());
}
