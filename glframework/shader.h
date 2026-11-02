#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../wrapper/checkError.h"
#include<string>
#include<fstream>
#include<sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




//本sharer类为GPU资源封装类
//提供着色器文件的加载
//uniform变量的传输


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

	void setMatrix44(const std::string& name, glm::mat4 value);

private:
	void checkShaderError(GLuint target , std::string type);

private:
	GLuint m_program{ 0 };
};