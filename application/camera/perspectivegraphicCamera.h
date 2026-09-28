#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"


class perspectivegraphicCamera :public Camera
{
public:

	perspectivegraphicCamera(float fov , float a , float n , float f);
	~perspectivegraphicCamera();

	//透视投影矩阵
	glm::mat4 getprojectionMatrix() override;

private:
	//透视投影观察盒数据
	float mFovy = 0.0f;
	float mAspect = 0.0f;
	float mNear = 0.0f;
	float mFar = 0.0f;
};