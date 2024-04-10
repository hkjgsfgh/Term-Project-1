#include "Application.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>



// 初始化Application的静态变量
Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
	// 如果mInstance已经被实例化了,则直接返回mInstance,否则new出来之后再返回
	if (mInstance == nullptr) {
		mInstance = new Application();
	}
	return mInstance;
}

Application::Application() {

}

Application::~Application() {

}

bool Application::init(const int& width, const int& height) {
	mWidth = width;
	mHeight = height;

	// 1. 初始化glfw初始环境
	glfwInit();
	// 4.6版本(glad选择的版本)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

	// 2. 创建窗体对象
	mWindow = glfwCreateWindow(mWidth, mHeight, "Learn OpenGL", NULL, NULL); // 窗体对象
	if (mWindow == NULL) {
		return false;
	}
	glfwMakeContextCurrent(mWindow); // 设置当前窗体为opengl绘制的舞台

	

	// 使用glad加载所有当前版本需要的openGL函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "初始化GLAD失败" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(mWindow, frameSizeCallback);
	glfwSetKeyCallback(mWindow, frameKeyCallback);
	glfwSetCursorPosCallback(mWindow, frameMouseCallback);
	glfwSetScrollCallback(mWindow, frameScrollCallback);

	// this是当前全局唯一的Application对象指针
	glfwSetWindowUserPointer(mWindow, this);

	return true;
}

bool Application::update() {
	if (glfwWindowShouldClose(mWindow)) {
		return false;
	}

	// 接受并分发窗口消息
	// 检查消息队列是否有需要处理的鼠标,键盘等消息
	// 如果有的话就将消息批量处理,清空消息队列
	glfwPollEvents();

	// 切换双缓存
	glfwSwapBuffers(mWindow);

	return true;
}

void Application::destroy() {
	// 退出程序前做相关清理
	glfwTerminate();
	return;
}

void Application::frameSizeCallback(GLFWwindow* window, int width, int height) {
	std::cout << "Resize" << std::endl;
	/*if (Application::getInstance()->mResizeCallback != nullptr) {
		Application::getInstance()->mResizeCallback(width, height);
	}*/
	// 优雅的在static方法中调用方法
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mResizeCallback != nullptr) {
		self->mResizeCallback(width, height);
	}
	return;
}


void Application::frameKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mKeyCallback != nullptr) {
		self->mKeyCallback(key, scancode, action, mods);
	}
	return;
}

void Application::frameMouseCallback(GLFWwindow* window, double xpos, double ypos) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mMouseCallback != nullptr) {
		self->mMouseCallback(xpos, ypos);
	}
	return;
}

void Application::frameScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mScrollCallback != nullptr) {
		self->mScrollCallback(xoffset,yoffset);
	}
	return;
}