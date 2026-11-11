#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

enum class LightType
{
	Directional = 0, // 平行光
	Point = 1,       // 点光源
	Spot = 2         // 探照灯
};

class Light
{
public:
	Light();
	virtual ~Light();
public:
	// 当前灯光的类型（子类构造时需指定）
	LightType mLightType{};

	// 通用属性
	glm::vec3 mColor{ 1.0f, 0.98f, 0.96f }; // 光的颜色
	glm::vec3 mPosition{ 0.0f, 0.0f, 0.0f }; // 灯的位置 (平行光用不到，但点光源和探照灯必须用)
	float mSpecularIntensity{ 0.05f };       // 高光强度
};