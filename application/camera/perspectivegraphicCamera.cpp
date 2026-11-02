#include "perspectivegraphicCamera.h"

perspectivegraphicCamera::perspectivegraphicCamera(float fov, float a, float n, float f)
{
	mFovy = fov;
	mAspect = a;
	mNear = n;
	mFar = f;
}

perspectivegraphicCamera::~perspectivegraphicCamera()
{

}

glm::mat4 perspectivegraphicCamera::getprojectionMatrix()
{
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}