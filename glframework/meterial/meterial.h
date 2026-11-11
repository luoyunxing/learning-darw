#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

//枚举类
enum class MeterialType
{
	//类型一
	PhoneMeterial,
	WhiteMeterial,
	CyanMeterial
};

class Meterial
{
public:

	Meterial();
	~Meterial();

public:
	MeterialType mType;
};