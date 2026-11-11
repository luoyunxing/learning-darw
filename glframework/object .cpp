#include "object.h"

Object::Object()
{

}
Object::~Object()
{

}

void Object::setPosition(glm::vec3 Pos)
{
	mPosition = Pos;
}

//获取初始位置
glm::vec3 Object::getPosition()
{
	return mPosition;
}

void Object::setAngleX(float angle)
{
	mAngleX += angle;
}

void Object::setAngleY(float angle)
{
	mAngleY += angle;
}

void Object::setAngleZ(float angle)
{
	mAngleZ += angle;
}

void Object::setScale(glm::vec3 scale)
{
	mScale = scale;
}

glm::mat4 Object::getmodelMatrix()
{
	//unity标准  先缩放(基于自身）   旋转（基于自身）    最后平移（基于世界坐标系）
	glm::mat4 transform{ 1.0f };

	//缩放
	transform = glm::scale(transform, mScale);  

	//旋转
	transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0, 0.0, 0.0));
	transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0, 1.0, 0.0));
	transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0, 0.0, 1.0));

	//任意旋转
	transform = transform * mArbitraryRotation;

	//世界坐标系下平移 
	//                          世界坐标位置               * 刚刚经历的变换
	transform = glm::translate(glm::mat4(1.0f), mPosition) * transform;

	return transform;
}

void Object::rotateArbitraryAxis(float angleDegrees, glm::vec3 axis)
{
	// 每次调用会在这个变量上继续累乘
	mArbitraryRotation = glm::rotate(mArbitraryRotation, glm::radians(angleDegrees), glm::normalize(axis));
}