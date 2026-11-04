#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../wrapper/checkError.h"
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


class Object
{
public:
	Object();
	~Object();
	
	void setPosition(glm::vec3 Pos);

	//增量变换
	void setAngleX(float angle);
	void setAngleY(float angle);
	void setAngleZ(float angle);

	void setScale(glm::vec3 scale);

	glm::mat4 getmodelMatrix();
protected:
	glm::vec3 mPosition{ 0.0f };

	float mAngleX{ 0.0f };
	float mAngleY{ 0.0f };
	float mAngleZ{ 0.0f };

	glm::vec3 mScale{ 0.0f };
};