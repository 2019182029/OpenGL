#include "CObject.h"

Object::Object() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	m_mf4x4World = glm::mat4(1.0f);

	m_vf3Position = { 0.0f, 0.0f, 0.0f };
	m_vf3RotationSpeed = { 0.0f, 0.0f, 0.0f };
	m_vf3RevolutionPosition = { 0.0f, 0.0f, 0.0f };
	m_vf3RevolutionSpeed = { 0.0f, 0.0f, 0.0f };
	m_vf3ScaleRatio = { 1.0f, 1.0f, 1.0f };

	m_flength = 0.0f;

	m_pMesh = NULL;
	m_AABB[0] = glm::vec3{ 1.0f };
	m_AABB[1] = glm::vec3{ 1.0f };

	m_sType = "";
}

Object::Object(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	m_mf4x4World = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(fx, fy, fz));
	m_mf4x4World = T * m_mf4x4World;

	m_vf3Position = { fx, fy, fz };
	m_vf3RotationSpeed = { 0.0f, 0.0f, 0.0f };
	m_vf3RevolutionPosition = { 0.0f, 0.0f, 0.0f };
	m_vf3RevolutionSpeed = { 0.0f, 0.0f, 0.0f };
	m_vf3ScaleRatio = { 1.0f, 1.0f, 1.0f };

	m_flength = flength;

	m_pMesh = NULL;
	m_AABB[0] = glm::vec3{ 1.0f };
	m_AABB[1] = glm::vec3{ 1.0f };

	m_sType = "";
}

Object::~Object() {
	if (m_pMesh) {
		delete m_pMesh;
		m_pMesh = NULL;
	}
}

void Object::SetVbo() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_pMesh->m_vVertices.size() * sizeof(DiffusedVertex), m_pMesh->m_vVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pMesh->m_uiIndicies.size() * sizeof(unsigned int), m_pMesh->m_uiIndicies.data(), GL_STATIC_DRAW);
}

void Object::SetRotationSpeed(GLfloat fx, GLfloat fy, GLfloat fz) {
	m_vf3RotationSpeed = { fx, fy, fz };
}

void Object::SetRevolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian) {
	m_vf3RevolutionPosition = { fx, fy, fz };
	m_vf3RevolutionSpeed = { fxRadian, fyRadian, fzRadian };
}

void Object::Rotate(GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian) {
	glm::mat4 T = glm::mat4{ 1.0f }; T = glm::translate(T, -m_vf3Position);

	m_mf4x4World = T * m_mf4x4World;

	glm::mat4 Rx = glm::mat4{ 1.0f }; Rx = glm::rotate(Rx, glm::radians(fxRadian), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry = glm::mat4{ 1.0f }; Ry = glm::rotate(Ry, glm::radians(fyRadian), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rz = glm::mat4{ 1.0f }; Rz = glm::rotate(Rz, glm::radians(fzRadian), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 R = glm::mat4{ 1.0f }; R = R * Rz * Ry * Rx;

	m_mf4x4World = R * m_mf4x4World;

	T = glm::mat4{ 1.0f }; T = glm::translate(T, m_vf3Position);

	m_mf4x4World = T * m_mf4x4World;
}

void Object::Revolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian) {
	glm::mat4 T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(-fx, -fy, -fz));

	m_mf4x4World = T * m_mf4x4World;

	glm::mat4 Rx = glm::mat4{ 1.0f }; Rx = glm::rotate(Rx, glm::radians(fxRadian), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry = glm::mat4{ 1.0f }; Ry = glm::rotate(Ry, glm::radians(fyRadian), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rz = glm::mat4{ 1.0f }; Rz = glm::rotate(Rz, glm::radians(fzRadian), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 R = glm::mat4{ 1.0f }; R = R * Rz * Ry * Rx;

	m_mf4x4World = R * m_mf4x4World;

	T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(fx, fy, fz));

	m_mf4x4World = T * m_mf4x4World;

	m_vf3Position = { m_mf4x4World[3][0], m_mf4x4World[3][1], m_mf4x4World[3][2] };
}

void Object::Translate(GLfloat fx, GLfloat fy, GLfloat fz) {
	glm::mat4 T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(fx, fy, fz));

	m_mf4x4World = T * m_mf4x4World;

	m_vf3Position.x += fx;
	m_vf3Position.y += fy;
	m_vf3Position.z += fz;
}

void Object::Scale(GLfloat fx, GLfloat fy, GLfloat fz) {
	m_vf3ScaleRatio = { fx, fy, fz };
}

void Object::Update() {
	Rotate(m_vf3RotationSpeed.x, m_vf3RotationSpeed.y, m_vf3RotationSpeed.z);
	Revolution(m_vf3RevolutionPosition.x, m_vf3RevolutionPosition.y, m_vf3RevolutionPosition.z, m_vf3RevolutionSpeed.x, m_vf3RevolutionSpeed.y, m_vf3RevolutionSpeed.z);

	UpdateBoundingBox();
}

void Object::UpdateBoundingBox() {
	glm::mat4 mf4x4World = m_mf4x4World;

	if (m_vf3ScaleRatio.x != 1.0f || m_vf3ScaleRatio.y != 1.0f || m_vf3ScaleRatio.z != 1.0f) {
		glm::mat4 T1 = glm::mat4{ 1.0f }; T1 = glm::translate(T1, glm::vec3(-mf4x4World[3][0], -mf4x4World[3][1], -mf4x4World[3][2]));
		glm::mat4 R1 = m_mf4x4World; R1[3][0] = 0.0f; R1[3][1] = 0.0f; R1[3][2] = 0.0f; R1 = glm::transpose(R1);
		glm::mat4 S = glm::mat4{ 1.0f }; S = glm::scale(S, m_vf3ScaleRatio);
		glm::mat4 R2 = glm::transpose(R1);
		glm::mat4 T2 = glm::mat4{ 1.0f }; T2 = glm::translate(T2, glm::vec3(mf4x4World[3][0], mf4x4World[3][1], mf4x4World[3][2]));
		mf4x4World = T2 * R2 * S * R1 * T1 * mf4x4World;
	}

	auto minX = (*(std::min_element(m_pMesh->m_vVertices.begin(), m_pMesh->m_vVertices.end(), [&](const Vertex& v1, const Vertex& v2) {
		return (mf4x4World * glm::vec4(v1.x, v1.y, v1.z, 1.0f)).x < (mf4x4World * glm::vec4(v2.x, v2.y, v2.z, 1.0f)).x;
		}))).x;

	auto minY = (*(std::min_element(m_pMesh->m_vVertices.begin(), m_pMesh->m_vVertices.end(), [&](const Vertex& v1, const Vertex& v2) {
		return (mf4x4World * glm::vec4(v1.x, v1.y, v1.z, 1.0f)).y < (mf4x4World * glm::vec4(v2.x, v2.y, v2.z, 1.0f)).y;
		}))).y;

	auto minZ = (*(std::min_element(m_pMesh->m_vVertices.begin(), m_pMesh->m_vVertices.end(), [&](const Vertex& v1, const Vertex& v2) {
		return (mf4x4World * glm::vec4(v1.x, v1.y, v1.z, 1.0f)).z < (mf4x4World * glm::vec4(v2.x, v2.y, v2.z, 1.0f)).z;
		}))).z;

	m_AABB[0] = glm::vec3(mf4x4World * glm::vec4(minX, minY, minZ, 1.0f));

	auto maxX = (*(std::max_element(m_pMesh->m_vVertices.begin(), m_pMesh->m_vVertices.end(), [&](const Vertex& v1, const Vertex& v2) {
		return (mf4x4World * glm::vec4(v1.x, v1.y, v1.z, 1.0f)).x < (mf4x4World * glm::vec4(v2.x, v2.y, v2.z, 1.0f)).x;
		}))).x;

	auto maxY = (*(std::max_element(m_pMesh->m_vVertices.begin(), m_pMesh->m_vVertices.end(), [&](const Vertex& v1, const Vertex& v2) {
		return (mf4x4World * glm::vec4(v1.x, v1.y, v1.z, 1.0f)).y < (mf4x4World * glm::vec4(v2.x, v2.y, v2.z, 1.0f)).y;
		}))).y;

	auto maxZ = (*(std::max_element(m_pMesh->m_vVertices.begin(), m_pMesh->m_vVertices.end(), [&](const Vertex& v1, const Vertex& v2) {
		return (mf4x4World * glm::vec4(v1.x, v1.y, v1.z, 1.0f)).z < (mf4x4World * glm::vec4(v2.x, v2.y, v2.z, 1.0f)).z;
		}))).z;

	m_AABB[1] = glm::vec3(mf4x4World * glm::vec4(maxX, maxY, maxZ, 1.0f));

	//std::cout << m_AABB[0].x << " " << m_AABB[0].y << " " << m_AABB[0].z << std::endl;
	//std::cout << m_AABB[1].x << " " << m_AABB[1].y << " " << m_AABB[1].z << std::endl;
}

void Object::PrepareRender(GLuint iShaderProgramId) {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(2 * 3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glm::mat4 mf4x4World = m_mf4x4World;

	if (m_vf3ScaleRatio.x != 1.0f || m_vf3ScaleRatio.y != 1.0f || m_vf3ScaleRatio.z != 1.0f) {
		glm::mat4 T1 = glm::mat4{ 1.0f }; T1 = glm::translate(T1, glm::vec3(-mf4x4World[3][0], -mf4x4World[3][1], -mf4x4World[3][2]));
		glm::mat4 R1 = m_mf4x4World; R1[3][0] = 0.0f; R1[3][1] = 0.0f; R1[3][2] = 0.0f; R1 = glm::transpose(R1);
		glm::mat4 S = glm::mat4{ 1.0f }; S = glm::scale(S, m_vf3ScaleRatio);
		glm::mat4 R2 = glm::transpose(R1);
		glm::mat4 T2 = glm::mat4{ 1.0f }; T2 = glm::translate(T2, glm::vec3(mf4x4World[3][0], mf4x4World[3][1], mf4x4World[3][2]));
		mf4x4World = T2 * R2 * S * R1 * T1 * mf4x4World;
	}

	unsigned int modelLocation = glGetUniformLocation(iShaderProgramId, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mf4x4World));

	// 법선 벡터는 월드 변환 행렬의 역행렬의 전치 행렬을 전달
	glm::mat4 mf4x4Normal = glm::inverse(glm::transpose(m_mf4x4World));

	unsigned int normalLocation = glGetUniformLocation(iShaderProgramId, "normalTransform");
	glUniformMatrix4fv(normalLocation, 1, GL_FALSE, glm::value_ptr(mf4x4Normal));
}

void Object::Render(GLuint iShaderProgramId) {
	PrepareRender(iShaderProgramId);

	glDrawElements(GL_TRIANGLES, (GLsizei)(m_pMesh->m_uiIndicies.size()), GL_UNSIGNED_INT, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriangleObject::TriangleObject() : Object() {
	m_pMesh = new TriangleMesh();
	m_sType = "Triangle";
}

TriangleObject::TriangleObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Object(fx, fy, fz, flength) {
	m_pMesh = new TriangleMesh(flength, fr, fg, fb);
	m_sType = "Triangle";
}

SquareObject::SquareObject() : Object() {
	m_pMesh = new SquareMesh();
	m_sType = "Square";
}

SquareObject::SquareObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Object(fx, fy, fz, flength) {
	m_pMesh = new SquareMesh(flength, fr, fg, fb);
	m_sType = "Square";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CubeObject::CubeObject() : Object() {
	m_pMesh = new CubeMesh();
	m_sType = "Cube";
}

CubeObject::CubeObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Object(fx, fy, fz, flength) {
	m_pMesh = new CubeMesh(flength, fr, fg, fb);
	m_sType = "Cube";
}

SphereObject::SphereObject() : Object() {
	m_pMesh = new SphereMesh();
	m_sType = "Sphere";
}

SphereObject::SphereObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Object(fx, fy, fz, flength) {
	m_pMesh = new SphereMesh(flength, fr, fg, fb);
	m_sType = "Sphere";
}

PyramidObject::PyramidObject() : Object() {
	m_pMesh = new PyramidMesh();
	m_sType = "Pyramid";
}

PyramidObject::PyramidObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Object(fx, fy, fz, flength) {
	m_pMesh = new PyramidMesh(flength, fr, fg, fb);
	m_sType = "Pyramid";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HierarchyObject::HierarchyObject() : Object() {
	m_pQobj = gluNewQuadric();

	m_vf3RotationOffset = { 0.0f, 0.0f, 0.0f };
	m_vf3RevolutionOffset = { 0.0f, 0.0f, 0.0f };

	m_mf4x4Transform = glm::mat4{ 1.0f };

	m_pMesh = new CubeMesh();
	m_sType = "Hierarchy";

	m_sFrameName = "";
	m_pChildObject = NULL;
	m_pSiblingObject = NULL;
}

HierarchyObject::HierarchyObject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Object(fx, fy, fz, flength) {
	m_pQobj = gluNewQuadric();

	m_vf3RotationOffset = { 0.0f, 0.0f, 0.0f };
	m_vf3RevolutionOffset = { 0.0f, 0.0f, 0.0f };

	m_mf4x4Transform = glm::mat4{ 1.0f };

	m_pMesh = new CubeMesh(flength, fr, fg, fb);
	m_sType = "Hierarchy";

	m_sFrameName = "";
	m_pChildObject = NULL;
	m_pSiblingObject = NULL;
}

void HierarchyObject::SetFrameName(std::string sFrameName) {
	m_sFrameName = sFrameName;
}

void HierarchyObject::SetChildObject(Object* pChildObject) {
	pChildObject->SetVbo();
	m_pChildObject = pChildObject;
}

void HierarchyObject::SetSiblingObject(Object* pSiblingObject) {
	pSiblingObject->SetVbo();
	m_pSiblingObject = pSiblingObject;
}

void HierarchyObject::Rotate(GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian) {
	m_vf3RotationOffset.x += fxRadian;
	m_vf3RotationOffset.y += fyRadian;
	m_vf3RotationOffset.z += fzRadian;
}

void HierarchyObject::Revolution(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat fxRadian, GLfloat fyRadian, GLfloat fzRadian) {
	m_vf3RevolutionOffset.x += fxRadian;
	m_vf3RevolutionOffset.y += fyRadian;
	m_vf3RevolutionOffset.z += fzRadian;
}

void HierarchyObject::Update() {
	glm::mat4 mf4x4Parent = glm::mat4{ 1.0f };

	UpdateTransform(&mf4x4Parent);

	if (m_pChildObject) ((HierarchyObject*)m_pChildObject)->UpdateTransform(&m_mf4x4World);
}

void HierarchyObject::UpdateTransform(glm::mat4* pmf4x4Parent) {
	Rotate(m_vf3RotationSpeed.x, m_vf3RotationSpeed.y, m_vf3RotationSpeed.z);
	Revolution(m_vf3RevolutionPosition.x, m_vf3RevolutionPosition.y, m_vf3RevolutionPosition.z, m_vf3RevolutionSpeed.x, m_vf3RevolutionSpeed.y, m_vf3RevolutionSpeed.z);

	m_mf4x4Transform = glm::mat4{ 1.0f };

	glm::mat4 Rx = glm::mat4{ 1.0f }; Rx = glm::rotate(Rx, glm::radians(m_vf3RotationOffset.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry = glm::mat4{ 1.0f }; Ry = glm::rotate(Ry, glm::radians(m_vf3RotationOffset.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rz = glm::mat4{ 1.0f }; Rz = glm::rotate(Rz, glm::radians(m_vf3RotationOffset.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 R = glm::mat4{ 1.0f }; R = R * Rz * Ry * Rx;
	m_mf4x4Transform = R * m_mf4x4Transform;

	glm::mat4 T = glm::mat4{ 1.0f }; T = glm::translate(T, m_vf3Position);
	m_mf4x4Transform = T * m_mf4x4Transform;

	T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(-m_vf3RevolutionPosition));
	m_mf4x4Transform = T * m_mf4x4Transform;
	Rx = glm::mat4{ 1.0f }; Rx = glm::rotate(Rx, glm::radians(m_vf3RevolutionOffset.x), glm::vec3(1.0f, 0.0f, 0.0f));
	Ry = glm::mat4{ 1.0f }; Ry = glm::rotate(Ry, glm::radians(m_vf3RevolutionOffset.y), glm::vec3(0.0f, 1.0f, 0.0f));
	Rz = glm::mat4{ 1.0f }; Rz = glm::rotate(Rz, glm::radians(m_vf3RevolutionOffset.z), glm::vec3(0.0f, 0.0f, 1.0f));
	R = glm::mat4{ 1.0f }; R = R * Rz * Ry * Rx;
	m_mf4x4Transform = R * m_mf4x4Transform;
	T = glm::mat4{ 1.0f }; T = glm::translate(T, glm::vec3(m_vf3RevolutionPosition));
	m_mf4x4Transform = T * m_mf4x4Transform;

	m_mf4x4World = *pmf4x4Parent * m_mf4x4Transform;

	if (m_pSiblingObject) ((HierarchyObject*)m_pSiblingObject)->UpdateTransform(pmf4x4Parent);
	if (m_pChildObject) ((HierarchyObject*)m_pChildObject)->UpdateTransform(&m_mf4x4World);
}

void HierarchyObject::Render(GLuint iShaderProgramId) {
	Object::Render(iShaderProgramId);

	if (m_pSiblingObject) m_pSiblingObject->Render(iShaderProgramId);
	if (m_pChildObject) m_pChildObject->Render(iShaderProgramId);
}

Object* HierarchyObject::FindFrame(std::string sFrameName) {
	if (sFrameName == m_sFrameName) return this;

	Object* pHierarchyObject = NULL;

	if (m_pSiblingObject) if (pHierarchyObject = ((HierarchyObject*)m_pSiblingObject)->FindFrame(sFrameName)) return pHierarchyObject;
	if (m_pChildObject) if (pHierarchyObject = ((HierarchyObject*)m_pChildObject)->FindFrame(sFrameName)) return pHierarchyObject;

	return pHierarchyObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Qobject::Qobject() : Object() {
	m_pQobj = gluNewQuadric();
	m_pMesh = new Mesh();
	m_sType = "Qobj";
}

Qobject::Qobject(GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength) : Object(fx, fy, fz, flength) {
	m_pQobj = gluNewQuadric();
	m_pMesh = new Mesh();
	m_sType = "Qobj";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Axis::Axis() : Object() {
	m_pMesh = new AxisMesh();
	m_sType = "Axis";
}

void Axis::Render(GLuint iShaderProgramId) {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DiffusedVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int modelLocation = glGetUniformLocation(iShaderProgramId, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_mf4x4World));

	glDrawArrays(GL_LINES, 0, (GLsizei)(m_pMesh->m_vVertices.size()));
}