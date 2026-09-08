#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../wrapper/checkError.h"
#include<string>
#include<fstream>
#include<sstream>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	
	~Shader();

	void begin();//使用这个shader

	void end(); //结束使用

	//获取uniform变量
	void setFloat(const std::string& name, float value);

	void setColor(const std::string& name, float r, float g, float b);

	void setColorVector(const std::string& name, const float* values);

	void setInt(const std::string& name, int value);

private:
	void checkShaderError(GLuint target , std::string type);

private:
	GLuint m_program{ 0 };
};