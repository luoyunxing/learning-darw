#include "camera.h"


Camera::Camera()
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::getprojectionMatrix()
{
	return glm::identity<glm::mat4>();
}

//获取摄像机坐标系
glm::mat4 Camera::getViewMatrix()
{
	glm::vec3 front = glm::cross(mUp, mRight);
	glm::vec3 center = mPosition + front;
	
	return glm::lookAt(mPosition, center, mUp);
}