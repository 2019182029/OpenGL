#include "CPlayer.h"

Player::Player() {
	m_pMesh = new AirplaneMesh();
	m_sType = "Player";
}

Player::Player(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb, GLfloat fa) : Object(fx, fy, fz, flength) {
	m_pMesh = new AirplaneMesh(flength, fr, fg, fb, fa);
	m_sType = "Player";

	m_bIsBlowingUp = false;
	m_fElapsedTime = 0.0f;

	m_pExplosionMesh = new CubeMesh(0.05f, fr, fg, fb, fa);

	for (int i = 0; i < 64; ++i) {
		float theta = glm::linearRand(0.0f, glm::two_pi<float>());
		float phi = glm::linearRand(0.0f, glm::pi<float>());

		float x = std::cos(theta) * std::sin(phi);
		float y = std::sin(theta) * std::sin(phi);
		float z = std::cos(phi);

		float magnitude = glm::linearRand(2.5f, 5.0f);

		m_pvf3Vectors[i] = glm::vec3(x, y, z) * magnitude;  // 방사형 방향 벡터 초기화
	}

	m_iHP = 100;
	number_of_bullets = 20;
}

void Player::Update() {
	if (m_bIsBlowingUp) {
		m_fElapsedTime += 0.016f;

		for (int i = 0; i < 64; ++i) {
			m_pmf4x4Transforms[i] = m_mf4x4World;
			m_pmf4x4Transforms[i][3][0] += (m_pvf3Vectors[i].x * m_fElapsedTime);
			m_pmf4x4Transforms[i][3][1] += (m_pvf3Vectors[i].y * m_fElapsedTime);
			m_pmf4x4Transforms[i][3][2] += (m_pvf3Vectors[i].z * m_fElapsedTime);
		}
	}
	else {
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
}

void Player::PrepareExplosion() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_pExplosionMesh->m_vVertices.size() * sizeof(DiffusedVertex), m_pExplosionMesh->m_vVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pExplosionMesh->m_uiIndicies.size() * sizeof(unsigned int), m_pExplosionMesh->m_uiIndicies.data(), GL_STATIC_DRAW);

	m_bIsBlowingUp = true;
}

void Player::Render(GLuint iShaderProgramId) {
	if (m_bIsBlowingUp) {
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		for (int i = 0; i < 64; ++i) {
			unsigned int modelLocation = glGetUniformLocation(iShaderProgramId, "modelTransform");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_pmf4x4Transforms[i]));

			// 법선 벡터는 월드 변환 행렬의 역행렬의 전치 행렬을 전달
			glm::mat4 mf4x4Normal = glm::inverse(glm::transpose(m_pmf4x4Transforms[i]));

			unsigned int normalLocation = glGetUniformLocation(iShaderProgramId, "normalTransform");
			glUniformMatrix4fv(normalLocation, 1, GL_FALSE, glm::value_ptr(mf4x4Normal));

			glDrawElements(GL_TRIANGLES, (GLsizei)(m_pExplosionMesh->m_uiIndicies.size()), GL_UNSIGNED_INT, 0);
		}
	}
	else {
		Object::PrepareRender(iShaderProgramId);

		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_pMesh->m_vVertices.size());
	}
}

void Player::Move_keydown(char direction) {
	switch (direction) {
	case 'w':			//상
		if (command_w == false) {

			ydir += 0.05f;
			command_w = true;
		}
		break;
	case 'a':			//좌
		if (command_a == false) {
			xdir += -0.05f;
			command_a = true;
		}
		break;
	case 's':			//하
		if (command_s == false) {
			ydir += -0.05f;
			command_s = true;
		}
		break;
	case 'd':			//우
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


Bullet::Bullet() {
	m_pMesh = new AirplaneMesh();
	m_sType = "Bullet";
}

Bullet::Bullet(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb, GLfloat fa) : Object(fx, fy, fz, flength) {
	m_pMesh = new AirplaneMesh(flength, fr, fg, fb, fa);
	m_sType = "Bullet";

	m_bIsBlowingUp = false;
	m_fElapsedTime = 0.0f;

	m_pExplosionMesh = new CubeMesh(0.05f, fr, fg, fb, fa);

	for (int i = 0; i < 64; ++i) {
		float theta = glm::linearRand(0.0f, glm::two_pi<float>());
		float phi = glm::linearRand(0.0f, glm::pi<float>());

		float x = std::cos(theta) * std::sin(phi);
		float y = std::sin(theta) * std::sin(phi);
		float z = std::cos(phi);

		float magnitude = glm::linearRand(2.5f, 5.0f);

		m_pvf3Vectors[i] = glm::vec3(x, y, z) * magnitude;  // 방사형 방향 벡터 초기화
	}
}

void Bullet::Update() {
	if (m_bIsBlowingUp) {
		m_fElapsedTime += 0.016f;

		for (int i = 0; i < 64; ++i) {
			m_pmf4x4Transforms[i] = m_mf4x4World;
			m_pmf4x4Transforms[i][3][0] += (m_pvf3Vectors[i].x * m_fElapsedTime);
			m_pmf4x4Transforms[i][3][1] += (m_pvf3Vectors[i].y * m_fElapsedTime);
			m_pmf4x4Transforms[i][3][2] += (m_pvf3Vectors[i].z * m_fElapsedTime);
		}
	}
	else {

		Translate(0, 0, -0.2);			//z축으로 0.2만큼 이동
		Object::Update();
	}
}

void Bullet::PrepareExplosion() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_pExplosionMesh->m_vVertices.size() * sizeof(DiffusedVertex), m_pExplosionMesh->m_vVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pExplosionMesh->m_uiIndicies.size() * sizeof(unsigned int), m_pExplosionMesh->m_uiIndicies.data(), GL_STATIC_DRAW);

	m_bIsBlowingUp = true;
}

void Bullet::Render(GLuint iShaderProgramId) {
	if (m_bIsBlowingUp) {
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		for (int i = 0; i < 64; ++i) {
			unsigned int modelLocation = glGetUniformLocation(iShaderProgramId, "modelTransform");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_pmf4x4Transforms[i]));

			// 법선 벡터는 월드 변환 행렬의 역행렬의 전치 행렬을 전달
			glm::mat4 mf4x4Normal = glm::inverse(glm::transpose(m_pmf4x4Transforms[i]));

			unsigned int normalLocation = glGetUniformLocation(iShaderProgramId, "normalTransform");
			glUniformMatrix4fv(normalLocation, 1, GL_FALSE, glm::value_ptr(mf4x4Normal));

			glDrawElements(GL_TRIANGLES, (GLsizei)(m_pExplosionMesh->m_uiIndicies.size()), GL_UNSIGNED_INT, 0);
		}
	}
	else {
		Object::PrepareRender(iShaderProgramId);

		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_pMesh->m_vVertices.size());
	}
}