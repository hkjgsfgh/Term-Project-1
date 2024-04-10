#pragma once
#include<iostream>

//ʹ�ú��Application��ʹ��
#define app Application::getInstance() 

/*
Application���ʾ��ǰӦ�ñ���	
��Ա����: (1) init(��ʼ��) (2)update(ÿһִ֡��) (3)destroy(��βִ��)
��Ӧ�ص�����Resize
	- ����һ������ָ��ResizeCallback
	- ����һ��ResizeCallback���͵ĳ�Ա����
	- ����һ��SetResizeCallback�ĺ���,���ô���仯����Ӧ�ص�����
	- ����һ��static��̬����,������Ӧglfw����仯
*/

class GLFWwindow;

// ��������ָ��; (*)��ʾ����һ������ָ��
using ResizeCallback = void(*) (int width, int height);
using KeyCallback = void(*) (int key, int scancode, int action, int mods);
using MouseCallback = void(*) (double xpos, double ypos);
using ScrollCallback = void(*) (double xoffset, double yoffset);

class Application {
public:
	~Application();
	// ���ڷ���ʵ���ľ�̬����
	static Application* getInstance();

	uint32_t getWidth()const {
		return mWidth;
	}

	uint32_t getHeight()const {
		return mHeight;
	}

	bool init(const int& width=800,const int& height=600);

	bool update();

	void destroy();

	void setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; }
	void setKeyCallback(KeyCallback callback) { mKeyCallback = callback; }
	void setMouseCallback(MouseCallback callback) { mMouseCallback = callback; }
	void setScrollCallback(ScrollCallback callback) { mScrollCallback = callback; }

	GLFWwindow* getWindow() { return mWindow; }

private:
	static void frameSizeCallback(GLFWwindow* window, int width, int height);
	static void frameKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void frameMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void frameScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


private:
	// ȫ��Ψһ�ľ�̬����ʵ��
	static Application* mInstance;

	uint32_t mWidth{ 0 };
	uint32_t mHeight{ 0 };
	GLFWwindow* mWindow{ nullptr };

	ResizeCallback mResizeCallback{ nullptr };
	KeyCallback mKeyCallback{ nullptr };
	MouseCallback mMouseCallback{ nullptr };
	ScrollCallback mScrollCallback{ nullptr };



	Application();

};