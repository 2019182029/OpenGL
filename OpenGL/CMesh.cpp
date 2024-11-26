#include "CMesh.h"

Mesh::Mesh() {
	m_flength = 0.1f;
}

Mesh::Mesh(GLfloat flength) {
	m_flength = flength;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh() : Mesh() {

}

TriangleMesh::TriangleMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Mesh(flength) {
	DiffusedVertex vertex = DiffusedVertex(0.0f, 2.0f / 3.0f * flength, 0.0f, fr, fg, fb);  // 0 : 꼭대기
	vertex.normal = { 0.0f, 0.0f, 1.0f };
	m_vVertices.emplace_back(vertex);

	vertex.x = -flength / sqrt(3.0f);  vertex.y = -flength / 3.0f;  // 1 : 정면 왼쪽 아래
	m_vVertices.emplace_back(vertex);

	vertex.x = flength / sqrt(3.0f);  // 2 : 정면 오른쪽 아래
	m_vVertices.emplace_back(vertex);

	// 정면
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(2);
}

SquareMesh::SquareMesh() : Mesh() {

}

SquareMesh::SquareMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Mesh(flength) {
	DiffusedVertex vertex = DiffusedVertex(-flength / 2.0f, flength / 2.0f, 0.0f, fr, fg, fb);  // 0 : 정면 왼쪽 위
	vertex.normal = { 0.0f, 0.0f, 1.0f };
	m_vVertices.emplace_back(vertex);

	vertex.y = -flength / 2.0f;  // 1 : 정면 왼쪽 아래
	m_vVertices.emplace_back(vertex);

	vertex.x = flength / 2.0f; vertex.y = flength / 2.0f;  // 2 : 정면 오른쪽 위
	m_vVertices.emplace_back(vertex);

	vertex.y = -flength / 2.0f;  // 3 : 정면 오른쪽 아래
	m_vVertices.emplace_back(vertex);

	// 정면 삼각형 1
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(2);

	// 정면 삼각형 2
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(3);
	m_uiIndicies.emplace_back(2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CubeMesh::CubeMesh() : Mesh() {

}

CubeMesh::CubeMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Mesh(flength) {
	DiffusedVertex vertex = DiffusedVertex(-flength / 2.0f, flength / 2.0f, flength / 2.0f, fr, fg, fb);  // 0 : 정면 왼쪽 위
	std::cout << vertex.r << " " << vertex.g << " " << vertex.b << std::endl;
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.y = -flength / 2.0f;  // 1 : 정면 왼쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.x = flength / 2.0f;  // 2 : 정면 오른쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.y = flength / 2.0f;  // 3 : 정면 오른쪽 위
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.x = -flength / 2.0f; vertex.z = -flength / 2.0f;  // 4 : 후면 왼쪽 위
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.y = -flength / 2.0f;  // 5 : 후면 왼쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.x = flength / 2.0f;  // 6 : 후면 오른쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.y = flength / 2.0f;  // 7: 후면 오른쪽 위
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	// 정면
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(2);

	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(2);
	m_uiIndicies.emplace_back(3);

	// 우측면
	m_uiIndicies.emplace_back(3);
	m_uiIndicies.emplace_back(2);
	m_uiIndicies.emplace_back(6);

	m_uiIndicies.emplace_back(3);
	m_uiIndicies.emplace_back(6);
	m_uiIndicies.emplace_back(7);

	// 좌측면
	m_uiIndicies.emplace_back(4);
	m_uiIndicies.emplace_back(5);
	m_uiIndicies.emplace_back(1);

	m_uiIndicies.emplace_back(4);
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(0);

	// 윗면
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(3);
	m_uiIndicies.emplace_back(4);

	m_uiIndicies.emplace_back(7);
	m_uiIndicies.emplace_back(4);
	m_uiIndicies.emplace_back(3);

	// 아랫면
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(5);
	m_uiIndicies.emplace_back(6);

	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(6);
	m_uiIndicies.emplace_back(2);

	// 후면
	m_uiIndicies.emplace_back(7);
	m_uiIndicies.emplace_back(6);
	m_uiIndicies.emplace_back(5);

	m_uiIndicies.emplace_back(7);
	m_uiIndicies.emplace_back(5);
	m_uiIndicies.emplace_back(4);
}

SphereMesh::SphereMesh() : Mesh() {

}

SphereMesh::SphereMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) {
	int nStacks = 20;
	int nSlices = 20;

	int nVertices = 2 + (nSlices * (nStacks - 1));

	float fDeltaPhi = float(3.1415927f / nStacks);
	float fDeltaTheta = float((2.0f * 3.1415927f) / nSlices);

	int k = 0;

	// 정점
	DiffusedVertex vertex = DiffusedVertex(0.0f, flength, 0.0f, fr, fg, fb);
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	float theta_i, phi_j;

	for (int j = 1; j < nStacks; j++) {
		phi_j = fDeltaPhi * j;
		for (int i = 0; i < nSlices; i++) {
			theta_i = fDeltaTheta * i;
			vertex.x = flength * sinf(phi_j) * cosf(theta_i); vertex.y = flength * cosf(phi_j); vertex.z = flength * sinf(phi_j) * sinf(theta_i);
			vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
			m_vVertices.emplace_back(vertex);
		}
	}

	vertex.x = 0.0f;  vertex.y = -flength;  vertex.z = 0.0f;
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	// 인덱스
	int nIndices = (nSlices * 3) * 2 + (nSlices * (nStacks - 2) * 3 * 2);

	k = 0;
	for (int i = 0; i < nSlices; i++) {
		m_uiIndicies.emplace_back(0);
		m_uiIndicies.emplace_back(1 + ((i + 1) % nSlices));
		m_uiIndicies.emplace_back(1 + i);
	}

	for (int j = 0; j < nStacks - 2; j++) {
		for (int i = 0; i < nSlices; i++) {
			m_uiIndicies.emplace_back(1 + (i + (j * nSlices)));
			m_uiIndicies.emplace_back(1 + (((i + 1) % nSlices) + (j * nSlices)));
			m_uiIndicies.emplace_back(1 + (i + ((j + 1) * nSlices)));

			m_uiIndicies.emplace_back(1 + (i + ((j + 1) * nSlices)));
			m_uiIndicies.emplace_back(1 + (((i + 1) % nSlices) + (j * nSlices)));
			m_uiIndicies.emplace_back(1 + (((i + 1) % nSlices) + ((j + 1) * nSlices)));
		}
	}

	for (int i = 0; i < nSlices; i++) {
		m_uiIndicies.emplace_back(nVertices - 1);
		m_uiIndicies.emplace_back(((nVertices - 1) - nSlices) + i);
		m_uiIndicies.emplace_back(((nVertices - 1) - nSlices) + ((i + 1) % nSlices));
	}
}

PyramidMesh::PyramidMesh() : Mesh() {

}

PyramidMesh::PyramidMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Mesh(flength) {
	DiffusedVertex vertex = DiffusedVertex(0.0f, flength * (float)sqrt(3) / 2.0f, 0.0f, fr, fg, fb);  // 0 : 꼭대기
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.x = -flength / 2.0f; vertex.y = -flength * (float)sqrt(3) / 6.0f; vertex.z = flength / 2.0f;  // 1 : 정면 왼쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.x = flength / 2.0f;  // 2 : 정면 오른쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.x = -flength / 2.0f; vertex.z = -flength / 2.0f;  // 3 : 후면 왼쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	vertex.x = flength / 2.0f;  // 3 : 후면 오른쪽 아래
	vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
	m_vVertices.emplace_back(vertex);

	// 정면
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(2);

	// 우측면
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(2);
	m_uiIndicies.emplace_back(4);

	// 좌측면
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(3);
	m_uiIndicies.emplace_back(1);

	// 후면
	m_uiIndicies.emplace_back(0);
	m_uiIndicies.emplace_back(4);
	m_uiIndicies.emplace_back(3);

	// 아랫면
	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(3);
	m_uiIndicies.emplace_back(4);

	m_uiIndicies.emplace_back(1);
	m_uiIndicies.emplace_back(4);
	m_uiIndicies.emplace_back(2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AirplaneMesh::AirplaneMesh() : Mesh() {

}

AirplaneMesh::AirplaneMesh(GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) : Mesh(flength) {
	float fx = flength * 0.6f, fy = flength * 0.6f, fz = flength * 0.1f;

	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);

	auto AddVertex = [&](float x, float y, float z) {
		DiffusedVertex vertex = DiffusedVertex(x, y, z, fr, fg, fb);
		vertex.normal = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
		m_vVertices.emplace_back(vertex);
		};

	// Upper Plane
	AddVertex(0.0f, +(fy + y3), -fz);
	AddVertex(+x1, -y1, -fz);
	AddVertex(0.0f, 0.0f, -fz);

	AddVertex(0.0f, +(fy + y3), -fz);
	AddVertex(0.0f, 0.0f, -fz);
	AddVertex(-x1, -y1, -fz);

	AddVertex(+x2, +y2, -fz);
	AddVertex(+fx, -y3, -fz);
	AddVertex(+x1, -y1, -fz);

	AddVertex(-x2, +y2, -fz);
	AddVertex(-x1, -y1, -fz);
	AddVertex(-fx, -y3, -fz);

	// Lower Plane
	AddVertex(0.0f, +(fy + y3), +fz);
	AddVertex(0.0f, 0.0f, +fz);
	AddVertex(+x1, -y1, +fz);

	AddVertex(0.0f, +(fy + y3), +fz);
	AddVertex(-x1, -y1, +fz);
	AddVertex(0.0f, 0.0f, +fz);

	AddVertex(+x2, +y2, +fz);
	AddVertex(+x1, -y1, +fz);
	AddVertex(+fx, -y3, +fz);

	AddVertex(-x2, +y2, +fz);
	AddVertex(-fx, -y3, +fz);
	AddVertex(-x1, -y1, +fz);

	// Left Plane
	AddVertex(0.0f, +(fy + y3), +fz);
	AddVertex(0.0f, +(fy + y3), -fz);
	AddVertex(-x2, +y2, -fz);

	AddVertex(0.0f, +(fy + y3), +fz);
	AddVertex(-x2, +y2, -fz);
	AddVertex(-x2, +y2, +fz);

	AddVertex(-x2, +y2, +fz);
	AddVertex(-x2, +y2, -fz);
	AddVertex(-fx, -y3, -fz);

	AddVertex(-x2, +y2, +fz);
	AddVertex(-fx, -y3, -fz);
	AddVertex(-fx, -y3, +fz);

	// Right Plane
	AddVertex(0.0f, +(fy + y3), -fz);
	AddVertex(0.0f, +(fy + y3), +fz);
	AddVertex(+x2, +y2, -fz);

	AddVertex(+x2, +y2, -fz);
	AddVertex(0.0f, +(fy + y3), +fz);
	AddVertex(+x2, +y2, +fz);

	AddVertex(+x2, +y2, -fz);
	AddVertex(+x2, +y2, +fz);
	AddVertex(+fx, -y3, -fz);

	AddVertex(+fx, -y3, -fz);
	AddVertex(+x2, +y2, +fz);
	AddVertex(+fx, -y3, +fz);

	// Back Plane
	AddVertex(+x1, -y1, -fz);
	AddVertex(+fx, -y3, -fz);
	AddVertex(+fx, -y3, +fz);

	AddVertex(+x1, -y1, -fz);
	AddVertex(+fx, -y3, +fz);
	AddVertex(+x1, -y1, +fz);

	AddVertex(0.0f, 0.0f, -fz);
	AddVertex(+x1, -y1, -fz);
	AddVertex(+x1, -y1, +fz);

	AddVertex(0.0f, 0.0f, -fz);
	AddVertex(+x1, -y1, +fz);
	AddVertex(0.0f, 0.0f, +fz);

	AddVertex(0.0f, 0.0f, -fz);
	AddVertex(0.0f, 0.0f, +fz);
	AddVertex(-x1, -y1, +fz);

	AddVertex(0.0f, 0.0f, -fz);
	AddVertex(-x1, -y1, +fz);
	AddVertex(-x1, -y1, -fz);

	AddVertex(-x1, -y1, -fz);
	AddVertex(-x1, -y1, +fz);
	AddVertex(-fx, -y3, +fz);

	AddVertex(-x1, -y1, -fz);
	AddVertex(-fx, -y3, +fz);
	AddVertex(-fx, -y3, -fz);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AxisMesh::AxisMesh() {
	DiffusedVertex vertex = DiffusedVertex(-100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);  // x축
	m_vVertices.emplace_back(vertex);

	vertex.x = 100.0f;
	m_vVertices.emplace_back(vertex);

	vertex.x = 0.0f; vertex.y = -100.0f;  // y축
	vertex.r = 0.0f; vertex.g = 1.0f;
	m_vVertices.emplace_back(vertex);

	vertex.y = 100.0f;
	m_vVertices.emplace_back(vertex);

	vertex.y = 0.0f; vertex.z = -100.0f;  // z축
	vertex.g = 0.0f; vertex.b = 1.0f;
	m_vVertices.emplace_back(vertex);

	vertex.z = 100.0f;
	m_vVertices.emplace_back(vertex);
}
