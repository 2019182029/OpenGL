#include "stdafx.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CCamera.h"

std::default_random_engine dre{ std::random_device{}() };
std::uniform_int_distribution<int> uid(1, 9);

char* filetobuf(const char* file);
void InitBuffer();
void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
//void add_object(std::string type, GLfloat fx = 0.0f, GLfloat fy = 0.0f, GLfloat fz = 0.0f, GLfloat flength = 0.1f, GLfloat fr = uid(dre) / 10.0f, GLfloat fg = uid(dre) / 10.0f, GLfloat fb = uid(dre) / 10.0f, GLfloat fa = 1.0f);

GLvoid draw_scene(GLvoid);
GLvoid reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y); 
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid Special(int key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid TimerFunction(int value);
GLvoid CheckPlayerObjectCollision();
GLvoid CheckBulletObjectCollision();

DWORD WINAPI PlaySoundEffect(LPVOID arg);

GLint width = 640, height = 640;
GLint shape = 0;

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgramID;

Object* pPlayer;
std::vector<Object*> walls;
std::vector<Object*> objects;
std::vector<Object*> bullets;
std::vector<Camera*> cameras;

glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
glm::vec3 lightPosition = { 0.0f, 0.0f, 1.0f };

GLfloat item = 0.1f;
GLfloat obstacle = 0.1f;

HANDLE hShootEvent;
HANDLE hExplosionEvent;

auto beforeTime = std::chrono::high_resolution_clock::now();
auto currentTime = std::chrono::high_resolution_clock::now();
auto itemTime = std::chrono::high_resolution_clock::now();
double elapsedTime = 0.0;
double reloadTime = 0.0;

auto startTime = std::chrono::high_resolution_clock::now();
auto endTime = std::chrono::high_resolution_clock::now();

int bonusScore = 0;
int totalScore = 0;

int main(int argc, char** argv) {
	// 윈도우 생성
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example");

	// GLEW 초기화
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "GLEW initialized" << std::endl;
	}

	make_shaderProgram();
	InitBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(draw_scene);
	//glutReshapeFunc(reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutTimerFunc(16, TimerFunction, 0);

	// 사운드 이벤트 객체 생성
	hShootEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hExplosionEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// 사운드 재생 스레드 생성
	HANDLE hSoundEffectThread;
	hSoundEffectThread = CreateThread(NULL, 0, PlaySoundEffect, NULL, 0, NULL);
	if (hSoundEffectThread != NULL) { CloseHandle(hSoundEffectThread); }

	glutMainLoop();
}

char* filetobuf(const char* file) {
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb"); /* Open file for reading */
	if (!fptr) return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
}

void InitBuffer() {
	Camera* pCamera = new ProjCamera();
	pCamera->SetView(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	pCamera->SetViewport(0, 0, width, height);
	((ProjCamera*)pCamera)->SetProj(glm::radians(90.0f), 1.0f, 0.01f, 25.0f);
	cameras.push_back(pCamera);

	glUseProgram(shaderProgramID);

	// 플레이어
	pPlayer = new Player(0.0f, -1.0f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f);
	pPlayer->SetVbo();
	pPlayer->Rotate(-90.0f, 0.0f, 0.0f);  // x축으로 -90 회전시켜 반대면

	// 벽
	Object* pObject = new SquareObject(0.0f, -2.0f, 0.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f);  // 아랫면
	pObject->Rotate(-90.0f, 0.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);

	pObject = new SquareObject(0.0f, -2.0f, -100.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f);  
	pObject->Rotate(-90.0f, 0.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);

	pObject = new SquareObject(2.0f, 0.0f, 0.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f);  // 오른면
	pObject->Rotate(0.0f, -90.0f, 0.0f);
	pObject->Rotate(-90.0f, 0.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);

	pObject = new SquareObject(2.0f, 0.0f, -100.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f);  
	pObject->Rotate(0.0f, -90.0f, 0.0f);
	pObject->Rotate(-90.0f, 0.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);

	pObject = new SquareObject(0.0f, 2.0f, 0.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f);  // 윗면
	pObject->Rotate(90.0f, 0.0f, 0.0f);
	pObject->Rotate(0.0f, 180.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);

	pObject = new SquareObject(0.0f, 2.0f, -100.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f); 
	pObject->Rotate(90.0f, 0.0f, 0.0f);
	pObject->Rotate(0.0f, 180.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);

	pObject = new SquareObject(-2.0f, 0.0f, 0.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f);  // 왼면
	pObject->Rotate(0.0f, 90.0f, 0.0f);
	pObject->Rotate(-90.0f, 0.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);

	pObject = new SquareObject(-2.0f, 0.0f, -100.0f, 100.0f, 0.5f, 0.5f, 0.5f, 1.0f); 
	pObject->Rotate(0.0f, 90.0f, 0.0f);
	pObject->Rotate(-90.0f, 0.0f, 0.0f);
	pObject->SetVbo();
	pObject->SetTexture("Wall.bmp");
	walls.emplace_back(pObject);
}

void make_vertexShaders() {
	GLchar* vertexSource;

	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);
	
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if(!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders() {
	GLchar* fragmentSource;
	
	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_shaderProgram() {
	make_vertexShaders(); 
	make_fragmentShaders();

	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgramID);
}

//void add_object(std::string stype, GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb, GLfloat fa) {
//	Object* pObject;
//
//	if (stype == "Triangle") { pObject = new TriangleObject(fx, fy, fz, flength, fr, fg, fb, fa); }
//	else if (stype == "Square") { pObject = new SquareObject(fx, fy, fz, flength, fr, fg, fb, fa); }
//	else if (stype == "Cube") { pObject = new CubeObject(fx, fy, fz, flength, fr, fg, fb, fa); }
//	else if (stype == "Sphere") { pObject = new SphereObject(fx, fy, fz, flength, fr, fg, fb, fa); }
//	else if (stype == "Pyramid") { pObject = new PyramidObject(fx, fy, fz, flength, fr, fg, fb, fa); }
//	else if (stype == "Hierarchy") { pObject = new HierarchyObject(fx, fy, fz, flength, fr, fg, fb, fa); }
//	else if (stype == "Q") { pObject = new Qobject(fx, fy, fz, flength); }
//	else if (stype == "Axis") { pObject = new Axis(); }
//
//	pObject->SetVbo();
//
//	objects.emplace_back(pObject);
//}

void SetOrthoProj() {
	// 현재 행렬 모드 저장
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 현재 뷰포트 가져오기
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// 투영 모드로 전환
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// 직교 투영 행렬 설정
	gluOrtho2D(0, viewport[2], 0, viewport[3]);

	// 모델뷰 모드로 다시 전환
	glMatrixMode(GL_MODELVIEW);
}

void RenderBitmapString(float x, float y, void* font, const char* string) {
	const char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void SetPerspectiveProj() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

GLvoid draw_scene(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform3f(glGetUniformLocation(shaderProgramID, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(shaderProgramID, "lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);

	for (const auto& cam : cameras) {
		cam->PrepareRender(shaderProgramID);

		glUniform3f(glGetUniformLocation(shaderProgramID, "viewPos"), cam->m_vf3Position.x, cam->m_vf3Position.y, cam->m_vf3Position.z);

		// 플레이어 렌더링
		if (!pPlayer->m_bTranslucent) { pPlayer->Render(shaderProgramID); }

		// 총알 렌더링
		for (const auto& obj : bullets) {  
			obj->Render(shaderProgramID);
		}

		// 벽 렌더링
		for (const auto& wall : walls) {
			wall->Render(shaderProgramID);
		}

		// 불투명한 객체 렌더링
		for (const auto& obj : objects) {
			if (!obj->m_bTranslucent) {
				obj->Render(shaderProgramID);
			}
		}

		// 반투명한 객체 렌더링
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (const auto& obj : objects) {
			if (obj->m_bTranslucent) {
				obj->Render(shaderProgramID);
			}
		}

		if (pPlayer->m_bTranslucent) { pPlayer->Render(shaderProgramID); }

		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	// 텍스트 렌더링
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int windowWidth = viewport[2];
	int windowHeight = viewport[3];

	SetOrthoProj();  // 직교 투영 설정

	glUseProgram(0);

	glPushMatrix(); 
	glLoadIdentity();

	// 텍스트 렌더링
	if (((Player*)pPlayer)->m_iHP != 0) {
		RenderBitmapString(10.0f, height - 25.0f, GLUT_BITMAP_HELVETICA_18, ("HP : " + std::to_string(((Player*)pPlayer)->m_iHP)).c_str());
		RenderBitmapString(10.0f, height - 50.0f, GLUT_BITMAP_HELVETICA_18, ("SCORE : " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000)).c_str());
		RenderBitmapString(10.0f, height - 75.0f, GLUT_BITMAP_HELVETICA_18, ("BONUS_SCORE : " + std::to_string(bonusScore)).c_str());
		RenderBitmapString(10.0f, height - 100.0f, GLUT_BITMAP_HELVETICA_18, ("BULLETS : " + std::to_string(((Player*)pPlayer)->number_of_bullets)).c_str());

	}
	else {
		RenderBitmapString(10.0f, height - 25.0f, GLUT_BITMAP_HELVETICA_18, (std::string("Game Over!")).c_str());
		RenderBitmapString(10.0f, height - 50.0f, GLUT_BITMAP_HELVETICA_18, ("SCORE : " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000)).c_str());
		RenderBitmapString(10.0f, height - 75.0f, GLUT_BITMAP_HELVETICA_18, ("BONUS_SCORE : " + std::to_string(bonusScore)).c_str());
		RenderBitmapString(10.0f, height - 100.0f, GLUT_BITMAP_HELVETICA_18, ("TOTAL_SCORE : " + std::to_string(totalScore)).c_str());
	}

	glPopMatrix();

	SetPerspectiveProj();  // 원근 투영 설정

	glUseProgram(shaderProgramID);

	glutSwapBuffers();
}

GLvoid reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'a':
	case 's':
	case 'd':
		static_cast<Player*>(pPlayer)->Move_keydown(key);
		break;

	case 'l':
		if (((Player*)pPlayer)->number_of_bullets > 0) {
			Object* pbullet;
			pbullet = new Bullet(pPlayer->m_vf3Position.x, pPlayer->m_vf3Position.y, pPlayer->m_vf3Position.z, 0.05f, 0.0f, 1.0f, 1.0f, 1.0f); 
			pbullet->m_bTranslucent = true;
			pbullet->Rotate(-90.0f, 0.0f, 0.0f);

			pbullet->SetVbo();
			bullets.emplace_back(pbullet);

			((Player*)pPlayer)->number_of_bullets -= 1;

			SetEvent(hShootEvent);
		}
		break;

	case 'q':
		for (auto& obj : objects) {
			((Obstacle*)obj)->PrepareExplosion();
		}
		break;

	default:
		break;
	}
}

GLvoid KeyboardUp(unsigned char key, int x, int y){
	switch (key) {
	case 'w':
	case 'a':
	case 's':
	case 'd':
		static_cast<Player*>(pPlayer)->Move_keyup(key);
		break;

	default:
		break;
	}
}

GLvoid Special(int key, int x, int y) {

}

GLvoid Mouse(int button, int state, int x, int y) {

}

GLvoid TimerFunction(int value) {
	// 장애물 생성
	if (glm::linearRand(0.0f, 10.0f) < obstacle) {
		Object* pObject;

		if (glm::linearRand(0.0f, 10.0f) < item) { pObject = new Obstacle(0.0f, 0.0f, -30.0f, 0.5f, uid(dre) / 10.0f, uid(dre) / 10.0f, uid(dre) / 10.0f, 0.5f); }  // 아이템
		else { pObject = new Obstacle(0.0f, 0.0f, -30.0f, 0.5f, uid(dre) / 10.0f, uid(dre) / 10.0f, uid(dre) / 10.0f, 1.0f); }  // 장애물
		
		pObject->SetVbo();
		pObject->SetRotationSpeed(glm::linearRand(-5.0f, 5.0f), glm::linearRand(-5.0f, 5.0f), glm::linearRand(-5.0f, 5.0f));

		objects.emplace_back(pObject);
	}

	// 플레이어 Update
	pPlayer->Update();

	// 플레이어의 아이템 지속 시간 종료 시
	if (pPlayer->m_bTranslucent) {
		if (((std::chrono::duration<double>)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - itemTime)).count() > 2.5f) {  // 2.5초
			for (auto& vertex : pPlayer->m_pMesh->m_vVertices) {
				vertex.a = 1.0f;
			}

			pPlayer->m_bTranslucent = false;
			pPlayer->SetVbo();
		}
	}

	// 벽 Update
	for (const auto& wall : walls) {
		wall->Translate(0.0f, 0.0f, 0.1f);

		// 무한 스크롤링
		if (wall->m_vf3Position.z > 50.0f) {
			wall->Translate(0.0f, 0.0f, -200.0f);
		}
	}
	
	// 장애물 Update
	for (const auto& obj : objects) {
		obj->Update();
	}
	
	// 총알 update
	for (const auto& obj : bullets) {
		obj->Update();
	}

	// 플레이어와 장애물 간의 충돌 검사
	CheckPlayerObjectCollision();

	// 플레이어의 총알과 장애물 간의 충돌 검사
	CheckBulletObjectCollision();

	// 카메라 뒤쪽으로 넘어갔거나 폭발 애니메이션이 끝난 장애물 제거
	objects.erase(std::remove_if(objects.begin(), objects.end(), [](const auto& obj) {
		if (((Obstacle*)obj)->m_vf3Position.z > 2.5f || ((Obstacle*)obj)->m_fElapsedTime > 2.5f) {
			delete obj;
			return true;
		}
		return false;
		}), objects.end());
	
	// 총알 제거
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const auto& obj) {
		if (((Bullet*)obj)->m_vf3Position.z < -30.0f) {
			delete obj;
			return true;
		}
		return false;
		}), bullets.end());

	// 반투명한 장애물 출력을 위한 Z값 기준 오름차순 정렬
	std::sort(objects.begin(), objects.end(), [](auto& obj1, auto& obj2) {
		return (*obj1).m_vf3Position.z < (*obj2).m_vf3Position.z;
		});

	// 1초마다 난이도 상승
	currentTime = std::chrono::high_resolution_clock::now();
	elapsedTime += ((std::chrono::duration<double>)std::chrono::duration_cast<std::chrono::microseconds>(currentTime - beforeTime)).count();
	reloadTime += ((std::chrono::duration<double>)std::chrono::duration_cast<std::chrono::microseconds>(currentTime - beforeTime)).count();
	beforeTime = std::chrono::high_resolution_clock::now();

	if (elapsedTime > 1.0) {
		obstacle += 0.01f;

		if (lightColor.x > 0.25f) {  // 임계값보다 낮아지지 않도록 설정
			lightColor = { lightColor.x - 0.01f, lightColor.y - 0.01f, lightColor.z - 0.01f };
		}

		elapsedTime = 0.0;
	}

	// 2초마다 총알 1발 충전
	if (reloadTime > 2.0) {
		if (((Player*)pPlayer)->number_of_bullets < 10) {
			((Player*)pPlayer)->number_of_bullets += 1;
		}

		reloadTime = 0.0;
	}

	// Score 갱신
	if (((Player*)pPlayer)->m_iHP != 0) {
		endTime = std::chrono::high_resolution_clock::now();
	}

	// 게임 종료 후 5초 경과 시 프로그램 종료
	if (((std::chrono::duration<double>)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - endTime)).count() > 5.0) {
		glutLeaveMainLoop();
	}

	totalScore = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000 + bonusScore;

	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 0);
}

GLvoid CheckPlayerObjectCollision() {
	for (auto& obj : objects) {
		if (((Player*)pPlayer)->m_bIsBlowingUp) { continue; }  // 플레이어가 폭발 중이 아니라면
		if (((Obstacle*)obj)->m_bIsBlowingUp) { continue; }  // 장애물이 폭발 중이 아니라면

		if (glm::distance(pPlayer->m_vf3Position, obj->m_vf3Position) > 0.5f) { continue; }  // 플레이어와 장애물 간의 거리가 0.5f 미만이라면
	
		if (obj->m_bTranslucent) {  // 장애물이 아이템이라면
			for (auto& vertex : pPlayer->m_pMesh->m_vVertices) {
				vertex.a = 0.25f;
			}

			pPlayer->m_bTranslucent = true;
			pPlayer->SetVbo();

			itemTime = std::chrono::high_resolution_clock::now();
		}
		else {  // 장애물이 아이템이 아니라면
			if (pPlayer->m_bTranslucent) { continue; }  // 플레이어에게 아이템 효과가 적용되고 있지 않다면

			if (((Player*)pPlayer)->m_iHP > 10) {  
				((Player*)pPlayer)->m_iHP -= 10;
			}
			else {
				((Player*)pPlayer)->m_iHP = 0;
				((Player*)pPlayer)->PrepareExplosion();  // 플레이어 사망

				SetEvent(hExplosionEvent);
			}
		}

		((Obstacle*)obj)->PrepareExplosion();  // 장애물 폭발

		SetEvent(hExplosionEvent);
	}
}

GLvoid CheckBulletObjectCollision() {
	for (auto& obj : objects) {
		if (((Obstacle*)obj)->m_bIsBlowingUp) { continue; }  // 장애물이 폭발 중이 아니라면

		for (auto& bullet : bullets) {
			if (glm::distance(obj->m_vf3Position, bullet->m_vf3Position) > 0.5f) { continue; }  // 플레이어와 장애물 간의 거리가 0.5f 미만이라면

			((Obstacle*)obj)->PrepareExplosion();  // 장애물 폭발

			SetEvent(hExplosionEvent);

			bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet));  // 총알 제거
			bonusScore += 1000;
		}
	}
}

DWORD __stdcall PlaySoundEffect(LPVOID arg) {
	// .wav 파일 open
	mciSendString(TEXT("open \"bgm.wav\" type mpegvideo alias bgm"), NULL, 0, NULL);
	mciSendString(TEXT("open \"shoot.wav\" type waveaudio alias shoot"), NULL, 0, NULL);
	mciSendString(TEXT("open \"explosion.wav\" type waveaudio alias explosion"), NULL, 0, NULL);

	// bgm 재생
	mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);

	while (1) {
		// shoot 재생
		if (WaitForSingleObject(hShootEvent, 0) == WAIT_OBJECT_0) {
			mciSendString(TEXT("play shoot from 0"), NULL, 0, NULL);
		}

		// explosion 재생
		if (WaitForSingleObject(hExplosionEvent, 0) == WAIT_OBJECT_0) {
			mciSendString(TEXT("play explosion from 0"), NULL, 0, NULL);
		}

		// 60fps에 맞춰 CPU 사용량 감소
		Sleep(16);
	}

	return 0;
}
