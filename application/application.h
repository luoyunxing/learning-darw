#pragma once
#include <cstdint>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//本application类
//GLFW 窗口与 OpenGL 上下文管理


//回调函数指针
using ResizeCallback = void(*)(int width, int height);
using KeyBoradCallback = void(*)(int key, int action, int mods);
using MouseCallback = void (*)(int button, int action, int mod);
using CursorCallback = void (*)(double xpos, double ypos);

class Application
{
public:
	~Application();

	//用于访问实例的静态函数
	static Application* getInstance();

	bool init(const int& width = 800, const int& height = 600);

	bool update();

	void destroy();

	uint32_t getWidth()const { return m_Width; }
	uint32_t getHeight()const { return m_Height; }
	GLFWwindow* getWindow()const { return m_window; }

	//外部函数接口
	
	//获取鼠标当前位置
	void getCursorPosition(double* x, double* y);
	
	//窗体
	void setResizeCallback(ResizeCallback callback)
	{
		m_ResizeCallback = callback; 
	}
	//键盘
	void setKeyBoradCallback(KeyBoradCallback callback)
	{
		m_KeyBoradCallback = callback;
	}
	//鼠标按键
	void setMouseCallback(MouseCallback callback)
	{
		m_MouseCallback = callback;
	}

	void setCursorCallback(CursorCallback callback)
	{
		m_CursorCallback = callback;
	}

private:
	//窗口大小回调函数
	static void frameBufferSizeCallBack(GLFWwindow* window, int width, int  height);
	//键盘响应回调函数
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	//鼠标按键回调函数
	static void mouseCallback(GLFWwindow* window, int button, int action, int mod);
	//光标回调函数
	static void cursorCallback(GLFWwindow* window, double xpos, double ypos);

private:
	//全局唯一静态实例
	static Application* mInstance;
	
	uint32_t m_Width{ 0 };
	uint32_t m_Height{ 0 };
	GLFWwindow* m_window{ nullptr };

	//接收外部函数
	ResizeCallback m_ResizeCallback{ nullptr };
	KeyBoradCallback m_KeyBoradCallback{ nullptr };
	MouseCallback m_MouseCallback{ nullptr };
	CursorCallback m_CursorCallback{ nullptr };

	Application();
};