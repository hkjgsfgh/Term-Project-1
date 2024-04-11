#include<iostream>
#include "glFramework/core.h"
#include "glFramework/shader.h"
#include<string>
#include<assert.h> // ����
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glFramework/texture.h"
#include "application/text.h"


std::string title = "2D Obejction";
std::string currentView = "-- Current View --";
std::string view = "Perspective";

// ����ͶӰ��ʽ,0Ϊ����ͶӰ,1Ϊ͸��ͶӰ
int projectionType = 1;


GLuint vao, program;
Texture* texture;
Shader* shader = nullptr;
Shader* textShader = nullptr;
Text* text = nullptr;


glm::mat4 transform(1.0f);
glm::mat4 viewMatrix(1.0f);
glm::mat4 projectionMatrix(1.0f);

// �������
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // ���������
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// ��������ʼλ��,����Ϊ��Ļ����
float lastX = app->getWidth() / 2.0f;
float lastY = app->getHeight() / 2.0f;

// ���ó�ʼ�����Ǻ�ƫ����
bool firstMouse = true; // ����һ�ν�����Ļ
float yaw = -90.0f;
float pitch = 0.0f;

// ���ó�ʼfov
float fov = 60.0f;

float deltaTime = 0.0;
float lastTime = 0.0;
float currentTime = 0.0;

// �����б��X��Y�����ϵ�offset
float XShear = 0.0;
float YShear = 0.0;


// ƽ��
void Translate(glm::vec3 value) {
	transform = glm::translate(glm::mat4(1.0f), value) * transform;
	return;
}

// ����
void Scale(glm::vec3 value) {
	transform = glm::scale(glm::mat4(1.0f), value) * transform;
	return;
}

// ��ת
void Rotation(float degree) {
	transform = glm::rotate(transform, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
	return;
}

// �б�
void Shear(std::string type,float value) {
	if (type == "X") {
		transform = glm::mat4(glm::vec4(1.0f, value, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * transform;
	}
	
	else if (type == "Y") {
		transform = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(value, 1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * transform;
	}
	return;
}

// ����
void Reflection(const std::string aspect) {
	if (aspect == "X") {
		transform = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, -1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * transform;
	}
	else if (aspect == "Y") {
		transform = glm::mat4(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * transform;
	}
	return;
}


// �������ڱ仯
void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	return;
}

void OnKey(int key, int scancode, int action, int mods) {
	// ������Ӧ�¼�
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		Reflection("X");
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		Reflection("Y");
	}
	
	// �ո��л�ͶӰ�ӽ�
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (projectionType == 1) {
			projectionType = 0;
			view = "Orthogonal";
		}
		else {
			projectionType = 1;
			view = "Perspective";
		}
	}
	return;
}

// ���ֿ��Ʒ�������
// yoffsetֵ������ֱ�����Ĵ�С
void OnScroll(double xoffset, double yoffset) {
	fov = fov - (float)yoffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 60.0f) {
		fov = 60.0f;
	}
	return;
}

// �����Ƹ����Ǻ�ƫ����
void OnMOuse(double xPosIn, double yPosIn) {
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	// ����һ�ν�����Ļ
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	// ����������
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// ���Ƹ����Ǻ�ƫ����
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(front);
	return;
}


void processTransform(GLFWwindow* window) {
	// ����esc�˳�����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// �����ƶ��ٶ�
	float speed = 5 * deltaTime;
	// �ƶ������
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos = cameraPos + cameraFront * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos = cameraPos - cameraFront * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos = cameraPos + glm::normalize(glm::cross(cameraUp, cameraFront)) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos = cameraPos - glm::normalize(glm::cross(cameraUp, cameraFront)) * speed;
	}

	// �ƶ�����
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		Translate(glm::vec3(-0.1f * speed, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		Translate(glm::vec3(0.1f * speed, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		Translate(glm::vec3(0.0f, 0.1f * speed, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		Translate(glm::vec3(0.0f, -0.1f * speed, 0.0f));
	}
	// ����
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		Scale(glm::vec3(1.0f + speed * 0.1f, 1.0f + speed * 0.1f, 1.0f + speed * 0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Scale(glm::vec3(1.0f - speed * 0.1f, 1.0f - speed * 0.1f, 1.0f - speed * 0.1f));
	}
	// ��ת
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		Rotation(15.0f * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		Rotation(-15.0f * speed);
	}
	// �б�
	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		XShear = 0.0;
		XShear += 0.1f * speed;
		Shear("X", XShear);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		XShear = 0.0;
		XShear -= 0.1f * speed;;
		Shear("X", XShear);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		YShear = 0.0;
		YShear += 0.1f * speed;;
		Shear("Y", YShear);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		YShear = 0.0;
		YShear -= 0.1f * speed;;
		Shear("Y", YShear);
	}

	return;
}


void prepareCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
	// lookAt: ����һ��viewMatrix
	// ����: eye:��ǰ��������ڵ�λ�� ; center:��ǰ�����������Ǹ��� ; up:񷶥����
	viewMatrix = glm::lookAt(pos, pos + front, up);
	return;
}

// ͸��ͶӰ
void preparePerspective(float fov) {
	// ����:fovy: y�᷽�����Ž�; aspect: ��ƽ��ĺ��ݰٷֱ�; 
	// near: ��ƽ����� ; far: Զƽ�����
	projectionMatrix = glm::perspective(glm::radians(fov), (float)app->getWidth() / (float)app->getHeight(), 0.1f, 1000.0f);
	return;
}

// ����ͶӰ
void prepareOrtho() {
	// ����: ��,��,��,��,��,Զ
	projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1000.0f, 1000.0f);
	return;
}

void prepareShader() {
	shader = new Shader("./assets/shaders/vertex_2D.glsl", "./assets/shaders/fragment_2D.glsl");
	return;
}

void prepareVAO() {
	// 1.׼������
	float position[] = {
	   -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	float color[] = {
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	// 2. ����VBO
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	// 3. ����EBO
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. ����VAO,��VAO
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 5. ��VBO,EBO,����������Ϣ
	// ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// ����EBO����ǰ��VAO����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // ���д��벻����ʡ��!

	// ����VAO
	glBindVertexArray(0);

	return;
}

void prepareTexture() {
	// ��һ������ͼƬ·��, �ڶ��������ǰ󶨵�����Ԫ
	texture = new Texture("./assets/textures/img.jpg", 0);
	return;
}

void render() {

	// ����ʱ���
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	processTransform(app->getWindow());
	prepareCamera(cameraPos, cameraFront, cameraUp);
	// �ж�ͶӰ��ʽ
	if (projectionType == 1) {
		preparePerspective(fov);
	}
	else if (projectionType == 0) {
		prepareOrtho();
	}

	// ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	// 1. �󶨵�ǰ��program
	shader->begin();

	// ͨ����fs�е�sampler����intֵ,ָ���ò�����ȥ�������Ų�����Ԫ
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("viewMatrix", viewMatrix);
	shader->setMatrix4x4("projectionMatrix", projectionMatrix);
	
	// 2. �󶨵�ǰ��vao
	GL_CALL(glBindVertexArray(vao));
	
	// 3. ��������ָ��
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	GL_CALL(glBindVertexArray(0));
	// ���
	shader->end();


	textShader->begin();
	textShader->setMatrix4x4("projection", glm::ortho(0.0f, static_cast<GLfloat>((float)app->getWidth()), 0.0f, static_cast<GLfloat>((float)app->getHeight())));
	textShader->begin();
	text->RenderText(*textShader, currentView, (float)(app->getWidth() / 2.0f - 100.0f), (float)app->getHeight() - 30.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), 1);
	text->RenderText(*textShader, view, (float)(app->getWidth() / 2.0f - 50.0f), (float)app->getHeight() - 60.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), 1);
	text->RenderText(*textShader, title, 0.0f, (float)app->getHeight() - 20.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), 1);

	return;
}


int main() {
	if (!app->init(800, 600)) {
		return -1;
	}

	// ���ü������ڴ�С�仯��Ϣ
	app->setResizeCallback(OnResize);
	app->setKeyCallback(OnKey);
	app->setMouseCallback(OnMOuse);
	app->setScrollCallback(OnScroll);

	// ����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	// Set OpenGL options
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ���ع�꣬����׽��
	glfwSetInputMode(app->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	prepareShader();
	prepareVAO();
	prepareTexture();
	prepareCamera(cameraPos, cameraFront, cameraUp);
	// Ĭ��Ϊ͸��ͶӰ
	preparePerspective(fov);


	textShader = new Shader("./assets/shaders/vertex_text.glsl", "./assets/shaders/fragment_text.glsl");
	text = new Text(0);
	text->loadText("./assets/fonts/font_1.ttf", 1);

	
	// 3. ִ�д���ѭ��
	while (app->update()) {
		// ��Ⱦ����
		render();
	}

	app->destroy();
	delete texture;

	return 0;
}
