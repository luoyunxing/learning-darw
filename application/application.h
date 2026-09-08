#pragma once
#include <cstdint>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//回调函数指针
using ResizeCallback = void(*)(int width, int height);
using KeyBoradCallback = void(*)(int key, int action, int mods);

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
	void setResizeCallback(ResizeCallback callback) { m_ResizeCallback = callback; }
	void setKeyBoradCallback(KeyBoradCallback callback) {m_KeyBoradCallback = callback; }

private:
	//窗口大小回调函数
	static void frameBufferSizeCallBack(GLFWwindow* window, int width, int  height);
	//键盘响应回调函数
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	//全局唯一静态实例
	static Application* mInstance;
	
	uint32_t m_Width{ 0 };
	uint32_t m_Height{ 0 };
	GLFWwindow* m_window{ nullptr };

	//接收外部函数
	ResizeCallback m_ResizeCallback{ nullptr };
	KeyBoradCallback m_KeyBoradCallback{ nullptr };

	Application();
};