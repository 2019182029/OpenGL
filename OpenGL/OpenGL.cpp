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
void add_object(std::string type, GLfloat fx = 0.0f, GLfloat fy = 0.0f, GLfloat fz = 0.0f, GLfloat flength = 0.1f, GLfloat fr = uid(dre) / 10.0f, GLfloat fg = uid(dre) / 10.0f, GLfloat fb = uid(dre) / 10.0f);

GLvoid draw_scene(GLvoid);
GLvoid reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y); 
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid Special(int key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid TimerFunction(int value);

GLint width = 640, height = 640;
GLint shape = 0;

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgramID;

Object* pPlayer;
std::vector<Object*> objects;
std::vector<Camera*> cameras;

glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
glm::vec3 lightPosition = { 0.0f, 0.0f, 1.0f };

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

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
	pCamera->SetView(glm::vec3(-1.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	pCamera->SetViewport(0, 0, width, height);
	((ProjCamera*)pCamera)->SetProj(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	cameras.push_back(pCamera);

	//Camera* pCamera = new OrthoCamera();
	//pCamera->SetView(glm::vec3(-1.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//pCamera->SetViewport(0, 0, width, height);
	//((OrthoCamera*)pCamera)->SetOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
	//cameras.push_back(pCamera);

	glUseProgram(shaderProgramID);

	pPlayer = new Player(0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f);
	pPlayer->SetVbo();
	pPlayer->Rotate(-90.0f, 0.0f, 0.0f);
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

void add_object(std::string stype, GLfloat fx, GLfloat fy, GLfloat fz, GLfloat flength, GLfloat fr, GLfloat fg, GLfloat fb) {
	Object* pObject;

	if (stype == "Triangle") { pObject = new TriangleObject(fx, fy, fz, flength, fr, fg, fb); }
	else if (stype == "Square") { pObject = new SquareObject(fx, fy, fz, flength, fr, fg, fb); }
	else if (stype == "Cube") { pObject = new CubeObject(fx, fy, fz, flength, fr, fg, fb); }
	else if (stype == "Sphere") { pObject = new SphereObject(fx, fy, fz, flength, fr, fg, fb); }
	else if (stype == "Pyramid") { pObject = new PyramidObject(fx, fy, fz, flength, fr, fg, fb); }
	else if (stype == "Hierarchy") { pObject = new HierarchyObject(fx, fy, fz, flength, fr, fg, fb); }
	else if (stype == "Q") { pObject = new Qobject(fx, fy, fz, flength); }
	else if (stype == "Axis") { pObject = new Axis(); }

	pObject->SetVbo();

	objects.emplace_back(pObject);
}

GLvoid draw_scene(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform3f(glGetUniformLocation(shaderProgramID, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(shaderProgramID, "lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);

	for (const auto& cam : cameras) {
		cam->PrepareRender(shaderProgramID);

		glUniform3f(glGetUniformLocation(shaderProgramID, "viewPos"), cam->m_vf3Position.x, cam->m_vf3Position.y, cam->m_vf3Position.z);

		pPlayer->Render(shaderProgramID);

		for (const auto& obj : objects) {
			obj->Render(shaderProgramID);
		}
	}

	glutSwapBuffers();
}

GLvoid reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {

}

GLvoid KeyboardUp(unsigned char key, int x, int y){

}

GLvoid Special(int key, int x, int y) {

}

GLvoid Mouse(int button, int state, int x, int y) {

}

GLvoid TimerFunction(int value) {
	for (const auto& obj : objects) {
		obj->Update();
	}

	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 0);
}
