#include "Application.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>



// ��ʼ��Application�ľ�̬����
Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
	// ���mInstance�Ѿ���ʵ������,��ֱ�ӷ���mInstance,����new����֮���ٷ���
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

	// 1. ��ʼ��glfw��ʼ����
	glfwInit();
	// 4.6�汾(gladѡ��İ汾)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // ���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // �ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ʹ�ú���ģʽ

	// 2. �����������
	mWindow = glfwCreateWindow(mWidth, mHeight, "Learn OpenGL", NULL, NULL); // �������
	if (mWindow == NULL) {
		return false;
	}
	glfwMakeContextCurrent(mWindow); // ���õ�ǰ����Ϊopengl���Ƶ���̨

	

	// ʹ��glad�������е�ǰ�汾��Ҫ��openGL����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "��ʼ��GLADʧ��" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(mWindow, frameSizeCallback);
	glfwSetKeyCallback(mWindow, frameKeyCallback);
	glfwSetCursorPosCallback(mWindow, frameMouseCallback);
	glfwSetScrollCallback(mWindow, frameScrollCallback);

	// this�ǵ�ǰȫ��Ψһ��Application����ָ��
	glfwSetWindowUserPointer(mWindow, this);

	return true;
}

bool Application::update() {
	if (glfwWindowShouldClose(mWindow)) {
		return false;
	}

	// ���ܲ��ַ�������Ϣ
	// �����Ϣ�����Ƿ�����Ҫ��������,���̵���Ϣ
	// ����еĻ��ͽ���Ϣ��������,�����Ϣ����
	glfwPollEvents();

	// �л�˫����
	glfwSwapBuffers(mWindow);

	return true;
}

void Application::destroy() {
	// �˳�����ǰ���������
	glfwTerminate();
	return;
}

void Application::frameSizeCallback(GLFWwindow* window, int width, int height) {
	std::cout << "Resize" << std::endl;
	/*if (Application::getInstance()->mResizeCallback != nullptr) {
		Application::getInstance()->mResizeCallback(width, height);
	}*/
	// ���ŵ���static�����е��÷���
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