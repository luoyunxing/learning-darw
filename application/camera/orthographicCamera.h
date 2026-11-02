#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"


class orthographicCamera :public Camera
{
public:
	orthographicCamera(float l , float r , float t , float b , float n , float f );

	~orthographicCamera();

	glm::mat4 getprojectionMatrix() override;

private:

	float mLeft{ 0.0f };
	float mRight{ 0.0f };
	float mTop{ 0.0f };
	float mBottom{ 0.0f };
	float mNear{ 0.0f };
	float mFar{ 0.0f };

};