#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Light
{
public:
	Light();
	~Light();
public:
	//光的颜色
	glm::vec3 mColor{ 1.0f, 0.98f, 0.96f };
	//高光强度
	float mSpecularIntensity{ 0.1f };
};