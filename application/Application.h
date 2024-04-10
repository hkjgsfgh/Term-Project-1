#pragma once
#include<iostream>

//使用宏简化Application的使用
#define app Application::getInstance() 

/*
Application类表示当前应用本身	
成员函数: (1) init(初始化) (2)update(每一帧执行) (3)destroy(结尾执行)
响应回调函数Resize
	- 声明一个函数指针ResizeCallback
	- 声明一个ResizeCallback类型的成员变量
	- 声明一个SetResizeCallback的函数,设置窗体变化的响应回调函数
	- 声明一个static静态函数,用于响应glfw窗体变化
*/

class GLFWwindow;

// 声明函数指针; (*)表示这是一个函数指针
using ResizeCallback = void(*) (int width, int height);
using KeyCallback = void(*) (int key, int scancode, int action, int mods);
using MouseCallback = void(*) (double xpos, double ypos);
using ScrollCallback = void(*) (double xoffset, double yoffset);

class Application {
public:
	~Application();
	// 用于访问实例的静态函数
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
	// 全局唯一的静态变量实例
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