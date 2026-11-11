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

	//设置初始位置（基于世界坐标系下）
	void setPosition(glm::vec3 Pos);
	//获取初始位置
	glm::vec3 getPosition();

	//增量变换 ， 初始旋转角度（基于自身）
	void setAngleX(float angle);
	void setAngleY(float angle);
	void setAngleZ(float angle);

	//任意旋转
	void rotateArbitraryAxis(float angleDegrees, glm::vec3 axis);

	//设置移动后的位置（基于世界坐标系）
	void setScale(glm::vec3 scale);

	//Object的模型变换
	glm::mat4 getmodelMatrix();
protected:
	//模型初始位置（世界坐标系下）
	glm::vec3 mPosition{ 0.0f };

	//任意旋转
	glm::mat4 mArbitraryRotation{ 1.0f };

	//初始旋转角度（基于自身）
	float mAngleX{ 0.0f };
	float mAngleY{ 0.0f };
	float mAngleZ{ 0.0f };

	//移动后的位置（基于世界坐标系）
	glm::vec3 mScale{ 1.0f };
};