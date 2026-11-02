#include "gameCameraControl.h"

GameCameraControl::GameCameraControl()
{

}
GameCameraControl::~GameCameraControl()
{

}

void GameCameraControl::onMousePress(int button, int action, double xpos, double ypos)
{
	
}


void GameCameraControl::onCursor(double xpos, double ypos)
{
	//x ， y 在屏幕上的变化值
	float deltaX = (xpos - mCurrentX) * mSensitivity;
	float deltaY = (ypos - mCurrentY) * mSensitivity;

	if (mLeftMouseDown)
	{
		pitch(-deltaY);
		yaw(-deltaX);
	}

	mCurrentX = xpos;
	mCurrentY = ypos;
}

void GameCameraControl::pitch(float angle)
{
	//向上看不能超过90度
	mAngleLimit += angle;
	if (mAngleLimit > 70.0f || mAngleLimit < -70.0f)
	{
		mAngleLimit -= angle;
		return;
	}

	//绕着mRight向量在旋转
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
}

void GameCameraControl::yaw(float angle)
{
	//绕世界Y轴旋转
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	//同时更改相机Right方向 和 Up方向
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
}

void GameCameraControl::update()
{
	glm::vec3 direction(0.0f);
	//前进方向为 ：右向量和头顶向量的叉乘
	glm::vec3 front = glm::cross(mCamera->mUp, mCamera->mRight);

	glm::vec3 right = mCamera->mRight;

	glm::vec3 up = mCamera->mUp;

	if (mKeyMap[GLFW_KEY_W])
	{
		direction += front;
	}

	if (mKeyMap[GLFW_KEY_A])
	{
		direction -= right;
	}

	if (mKeyMap[GLFW_KEY_S])
	{
		direction -= front;
	}

	if (mKeyMap[GLFW_KEY_D])
	{
		direction += right;
	}

	if (mKeyMap[GLFW_KEY_SPACE])
	{
		direction += up;
	}

	if (mKeyMap[GLFW_KEY_LEFT_SHIFT])
	{
		direction -= up;
	}

	if (glm::length(direction) != 0)
	{
		//对向量进行归一化处理
		direction = glm::normalize(direction);
		//更改相机位置
		mCamera->mPosition += direction * m_wasd_speed;
	}

}

