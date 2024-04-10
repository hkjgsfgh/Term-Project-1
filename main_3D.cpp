#include<iostream>
#include "glFramework/core.h"
#include "glFramework/shader.h"
#include<string>
#include<assert.h> // ����
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glFramework/texture.h"
#include<cmath>


// ����ͶӰ��ʽ,0Ϊ����ͶӰ,1Ϊ͸��ͶӰ
int projectionType = 1;


GLuint vao;
GLuint program;
Texture* texture;
Shader* shader = nullptr;
glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 viewMatrix(1.0f);
glm::mat4 projectionMatrix(1.0f);

// �������
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
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


// �������ڱ仯
void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	return;
}

void OnKey(int key, int scancode, int action, int mods) {
	// ������Ӧ�¼�
	// �ո��л�ͶӰ�ӽ�
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (projectionType == 1) {
			projectionType = 0;
		}
		else {
			projectionType = 1;
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
	float speed = 20 * deltaTime;
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
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		Translate(glm::vec3(0.0f, 0.0f, -0.1f * speed));
	}
	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		Translate(glm::vec3(0.0f, 0.0f, 0.1f * speed));
	}
	// ����
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		Scale(glm::vec3(1.0f + speed * 0.1f, 1.0f + speed * 0.1f, 1.0f + speed * 0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Scale(glm::vec3(1.0f - speed * 0.1f, 1.0f - speed * 0.1f, 1.0f - speed * 0.1f));
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
	projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1000.0f, -1000.0f);
	return;
}

void prepareShader() {
	shader = new Shader("./assets/shaders/vertex_3D.glsl", "./assets/shaders/fragment_3D.glsl");
	return;
}

void prepareVAO() {
	// 1.׼������
	float uvs[] = {
		1.0f, 0.0f,		
		0.0f, 0.0f,		
		0.5f, 1.0f
	};

	float position[] = {
	-1.0f, -1.0f, -1.0f,  // 0
	1.0f, -1.0f, -1.0f,   // 1
	1.0f, 1.0f, -1.0f,    // 2
	-1.0f, -1.0f, -1.0f,  // 0
	1.0f, 1.0f, -1.0f,    // 2
	-1.0f, 1.0f, -1.0f,   // 3

	-1.0f, -1.0f, -1.0f,  // 0
	-1.0f, 1.0f, -1.0f,   // 3
	-1.0f, 1.0f, 1.0f,    // 7
	-1.0f, -1.0f, -1.0f,  // 0
	-1.0f, 1.0f, 1.0f,    // 7
	-1.0f, -1.0f, 1.0f,   // 4

	1.0f, 1.0f, -1.0f,    // 2
	-1.0f, 1.0f, -1.0f,   // 3
	-1.0f, 1.0f, 1.0f,    // 7
	1.0f, 1.0f, -1.0f,    // 2
	-1.0f, 1.0f, 1.0f,    // 7
	1.0f, 1.0f, 1.0f,     // 6

	-1.0f, 1.0f, 1.0f,    // 7
	-1.0f, -1.0f, 1.0f,   // 4
	1.0f, 1.0f, 1.0f,     // 6
	-1.0f, -1.0f, 1.0f,   // 4
	1.0f, -1.0f, 1.0f,    // 5
	1.0f, 1.0f, 1.0f,     // 6

	-1.0f, -1.0f, -1.0f,  // 0
	1.0f, -1.0f, -1.0f,   // 1
	-1.0f, -1.0f, 1.0f,   // 4
	1.0f, -1.0f, -1.0f,   // 1
	-1.0f, -1.0f, 1.0f,   // 4
	1.0f, -1.0f, 1.0f,    // 5

	1.0f, 1.0f, -1.0f,    // 2
	1.0f, -1.0f, 1.0f,    // 5
	1.0f, 1.0f, 1.0f,     // 6
	1.0f, 1.0f, -1.0f,    // 2
	1.0f, -1.0f, 1.0f,    // 5
	1.0f, -1.0f, -1.0f,   // 1
	};

	float color[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,//

		0.0f, 1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,//

		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,//

		1.0f, 1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,// 

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
 
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f
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

	//// 3. ����EBO
	//GLuint ebo;
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

	//// ����EBO����ǰ��VAO����
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // ���д��벻����ʡ��!

	// ����VAO
	glBindVertexArray(0);

	return;
}

void prepareTexture() {
	texture = new Texture("./assets/textures/img.jpg",0);
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
	//GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
	//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	GL_CALL(glBindVertexArray(0));
	// ���
	shader->end();

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

	// ���ع�꣬����׽��
	glfwSetInputMode(app->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	prepareShader();
	prepareVAO();
	prepareTexture();
	prepareCamera(cameraPos, cameraFront, cameraUp);
	// Ĭ��Ϊ͸��ͶӰ
	preparePerspective(fov);

	// ȷ�Ͽ�����Ȼ���
	glEnable(GL_DEPTH_TEST);
	// 3. ִ�д���ѭ��
	while (app->update()) {
		// ��Ⱦ����
		render();
	}

	app->destroy();
	delete texture;

	return 0;
}
