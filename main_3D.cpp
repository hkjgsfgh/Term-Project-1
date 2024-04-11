#include<iostream>
#include "glFramework/core.h"
#include "glFramework/shader.h"
#include<string>
#include<assert.h> // 断言
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glFramework/texture.h"
#include "application\text.h"
#include<cmath>


std::string title = "3D Objection";
std::string currentView = "-- Current View --";
std::string view = "Perspective";


// 默认旋转轴为Y轴
std::string currentAxis = "Y";
std::string rotateAxisDescription = "The current Rotation Axis is: " + currentAxis +"-axis";


// 设置投影方式,0为正交投影,1为透视投影
int projectionType = 1;

GLuint vao;
GLuint program;
Texture* texture;
Shader* shader = nullptr;
Shader* textShader = nullptr;
Text* text = nullptr;


glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 viewMatrix(1.0f);
glm::mat4 projectionMatrix(1.0f);

// 相机参数
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 相机看向方向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 设置鼠标初始位置,设置为屏幕中心
float lastX = app->getWidth() / 2.0f;
float lastY = app->getHeight() / 2.0f;

// 设置初始俯仰角和偏航角
bool firstMouse = true; // 鼠标第一次进入屏幕
float yaw = -90.0f;
float pitch = 0.0f;

// 设置初始fov
float fov = 60.0f;

float deltaTime = 0.0;
float lastTime = 0.0;
float currentTime = 0.0;

// 平移
void Translate(glm::vec3 value) {
	transform = glm::translate(glm::mat4(1.0f), value) * transform;
	return;
}

// 缩放
void Scale(glm::vec3 value) {
	transform = glm::scale(glm::mat4(1.0f), value) * transform;
	return;
}

// 旋转
void Rotation(float degree) {
	if (currentAxis == "X") {
		transform = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(1.0f, 0.0f, 0.0f)) * transform;
	}
	else if (currentAxis == "Y") {
		transform = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f)) * transform;
	}
	else if (currentAxis == "Z") {
		transform = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f)) * transform;
	} 
	
	return;
}


// 监听窗口变化
void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	return;
}

void OnKey(int key, int scancode, int action, int mods) {
	// 键盘响应事件
	// 空格切换投影视角
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
	// 切换旋转轴
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		currentAxis = "X";
		rotateAxisDescription = "The current Rotation Axis is: " + currentAxis + "-axis";
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		currentAxis = "Y";
		rotateAxisDescription = "The current Rotation Axis is: " + currentAxis + "-axis";
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		currentAxis = "Z";
		rotateAxisDescription = "The current Rotation Axis is: " + currentAxis + "-axis";
	}
	return;
}

// 滚轮控制方法缩放
// yoffset值代表竖直滚动的大小
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

// 鼠标控制俯仰角和偏航角
void OnMOuse(double xPosIn, double yPosIn) {
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	// 鼠标第一次进入屏幕
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	// 设置灵敏度
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// 限制俯仰角和偏航角
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
	// 按下esc退出窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// 设置移动速度
	float speed = 20 * deltaTime;
	// 移动摄像机
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
	// 移动物体
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
	// 缩放
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		Scale(glm::vec3(1.0f + speed * 0.1f, 1.0f + speed * 0.1f, 1.0f + speed * 0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Scale(glm::vec3(1.0f - speed * 0.1f, 1.0f - speed * 0.1f, 1.0f - speed * 0.1f));
	}

	// 旋转
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		Rotation(3.0f * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		Rotation(-3.0f * speed);
	}
	return;
}


void prepareCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
	// lookAt: 生成一个viewMatrix
	// 参数: eye:当前摄像机所在的位置 ; center:当前摄像机看向的那个点 ; up:穹顶向量
	viewMatrix = glm::lookAt(pos, pos + front, up);
	return;
}

// 透视投影
void preparePerspective(float fov) {
	// 参数:fovy: y轴方向视张角; aspect: 近平面的横纵百分比; 
	// near: 近平面距离 ; far: 远平面距离
	projectionMatrix = glm::perspective(glm::radians(fov), (float)app->getWidth() / (float)app->getHeight(), 0.1f, 1000.0f);
	return;
}

// 正交投影
void prepareOrtho() {
	// 参数: 左,右,下,上,近,远
	projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1000.0f, 1000.0f);
	return;
}

void prepareShader() {
	shader = new Shader("./assets/shaders/vertex_3D.glsl", "./assets/shaders/fragment_3D.glsl");
	return;
}

void prepareVAO() {
	// 1.准备数据
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



	// 2. 创建VBO
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

	//// 3. 创建EBO
	//GLuint ebo;
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. 创建VAO,绑定VAO
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 5. 绑定VBO,EBO,加入描述信息
	// 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//// 加入EBO到当前的VAO当中
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // 此行代码不允许省略!

	// 清理VAO
	glBindVertexArray(0);

	return;
}

void prepareTexture() {
	texture = new Texture("./assets/textures/img.jpg",0);
	return;
}

void render() {
	// 计算时间差
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	processTransform(app->getWindow());
	prepareCamera(cameraPos, cameraFront, cameraUp);
	// 判断投影方式
	if (projectionType == 1) {
		preparePerspective(fov);
	}
	else if (projectionType == 0) {
		prepareOrtho();
	}

	// 执行opengl画布清理操作
	//GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// 1. 绑定当前的program
	shader->begin();

	// 通过将fs中的sampler赋于int值,指定该采样器去采样几号采样单元
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("viewMatrix", viewMatrix);
	shader->setMatrix4x4("projectionMatrix", projectionMatrix);

	// 2. 绑定当前的vao
	GL_CALL(glBindVertexArray(vao));
	// 3. 发出绘制指令
	//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	GL_CALL(glBindVertexArray(0));
	// 解绑
	shader->end();


	textShader->begin();
	textShader->setMatrix4x4("projection", glm::ortho(0.0f, static_cast<GLfloat>((float)app->getWidth()), 0.0f, static_cast<GLfloat>((float)app->getHeight())));
	textShader->begin();
	text->RenderText(*textShader, currentView, (float)(app->getWidth() / 2.0f - 100.0f), (float)app->getHeight() - 30.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), 1);
	text->RenderText(*textShader, view, (float)(app->getWidth() / 2.0f - 50.0f), (float)app->getHeight() - 60.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), 1);
	text->RenderText(*textShader, title, 0.0f, (float)app->getHeight() - 20.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), 1);
	text->RenderText(*textShader, rotateAxisDescription, (float)(app->getWidth() / 2.0f - 160.0f), 10.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), 1);
	
	return;
}


int main() {
	if (!app->init(800, 600)) {
		return -1;
	}

	// 设置监听窗口大小变化消息
	app->setResizeCallback(OnResize);
	app->setKeyCallback(OnKey);
	app->setMouseCallback(OnMOuse);
	app->setScrollCallback(OnScroll);

	// 设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	// 隐藏光标，并捕捉它
	glfwSetInputMode(app->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	prepareShader();
	prepareVAO();
	prepareTexture();
	prepareCamera(cameraPos, cameraFront, cameraUp);
	// 默认为透视投影
	preparePerspective(fov);

	textShader = new Shader("./assets/shaders/vertex_text.glsl", "./assets/shaders/fragment_text.glsl");
	text = new Text(0);
	text->loadText("./assets/fonts/font_1.ttf", 1);

	// 确认开启深度缓冲
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 3. 执行窗体循环
	while (app->update()) {
		// 渲染操作
		render();
	}

	app->destroy();
	delete texture;

	return 0;
}
