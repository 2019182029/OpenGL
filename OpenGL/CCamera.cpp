#include "CCamera.h"

Camera::Camera() : Object() {
	m_vf3At = { 0.0f, 0.0f, 0.0f };
	m_vf3Up = { 0.0f, 0.0f, 0.0f };

	m_mf4x4View = glm::mat4{ 1.0f };
	m_mf4x4Proj = glm::mat4{ 1.0f };

	m_ixViewport = 0;
	m_iyViewport = 0;
	m_iWidth = 0;
	m_iHeight = 0;
}

void Camera::SetView(glm::vec3 vf3Eye, glm::vec3 vf3At) {
	Object::Translate(vf3Eye.x, vf3Eye.y, vf3Eye.z);

	m_vf3Position = vf3Eye;
	m_vf3At = vf3At;
	m_vf3Up = { 0.0f, 1.0f, 0.0f };

	m_mf4x4View = glm::lookAt(m_vf3Position, m_vf3At, m_vf3Up);
}

void Camera::SetViewport(GLint ix, GLint iy, GLint iwidth, GLint iheight) {
	m_ixViewport = ix;
	m_iyViewport = iy;
	m_iWidth = iwidth;
	m_iHeight = iheight;
}

void Camera::Rotate(GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian) {
	glm::mat4 T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(-m_vf3Position.x, -m_vf3Position.y, -m_vf3Position.z));

	m_vf3At = glm::vec3(T * glm::vec4(m_vf3At, 1.0f));

	glm::mat4 Rx = glm::mat4{ 1.0f }; Rx = glm::rotate(Rx, glm::radians(fxRadian), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry = glm::mat4{ 1.0f }; Ry = glm::rotate(Ry, glm::radians(fyRadian), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rz = glm::mat4{ 1.0f }; Rz = glm::rotate(Rz, glm::radians(fzRadian), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 R = glm::mat4{ 1.0f }; R = R * Rz * Ry * Rx;

	m_vf3At = glm::vec3(R * glm::vec4(m_vf3At, 1.0f));

	T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(m_vf3Position.x, m_vf3Position.y, m_vf3Position.z));

	m_vf3At = glm::vec3(T * glm::vec4(m_vf3At, 1.0f));

	m_mf4x4View = glm::lookAt(m_vf3Position, m_vf3At, m_vf3Up);
}

void Camera::Revolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian) {
	Object::Revolution(fx, fy, fz, fxRadian, fyRadian, fzRadian);
	m_mf4x4View = glm::lookAt(m_vf3Position, m_vf3At, m_vf3Up);
}

void Camera::Translate(GLfloat fx, GLfloat fy, GLfloat fz) {
	Object::Translate(fx, fy, fz);
	m_mf4x4View = glm::lookAt(m_vf3Position, m_vf3At, m_vf3Up);
}

void Camera::PrepareRender(GLuint iShaderProgramId) {
	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(m_ixViewport, m_iyViewport, m_iWidth, m_iHeight);

	GLuint viewLocation = glGetUniformLocation(iShaderProgramId, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_mf4x4View));

	GLuint projLocation = glGetUniformLocation(iShaderProgramId, "projTransform");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(m_mf4x4Proj));
}

OrthoCamera::OrthoCamera() : Camera() {
	m_fLeft = 0.0f;
	m_fRight = 0.0f;
	m_fBottom = 0.0f;
	m_fTop = 0.0f;
	m_fNear = 0.0f;
	m_fFar = 0.0f;
}

void OrthoCamera::SetOrtho(GLfloat fLeft, GLfloat fRight, GLfloat fBottom, GLfloat fTop, GLfloat fNear, GLfloat fFar) {
	m_fLeft = fLeft;
	m_fRight = fRight;
	m_fBottom = fBottom;
	m_fTop = fTop;
	m_fNear = fNear;
	m_fFar = fFar;

	m_mf4x4Proj = glm::ortho(fLeft, fRight, fBottom, fTop, fNear, fFar);
}

ProjCamera::ProjCamera() : Camera() {
	m_fFovy = 0.0f;
	m_fAspect = 0.0f;
	m_fNear = 0.0f;
	m_fFar = 0.0f;
}

void ProjCamera::SetProj(GLfloat fFovy, GLfloat fAspect, GLfloat fNear, GLfloat fFar) {
	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_mf4x4Proj = glm::perspective(m_fFovy, m_fAspect, m_fNear, m_fFar);
}
