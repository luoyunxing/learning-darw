#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

enum class MeterialType
{
	PhoneMeterial
};

class Meterial
{
public:

	Meterial();
	~Meterial();

public:

	MeterialType mType;
};