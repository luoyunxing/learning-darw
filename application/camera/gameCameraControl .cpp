#include "gameCameraControl.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GameCameraControl::GameCameraControl()
{
	// 初始化一下初始角度
	m_yaw = -90.0f;
	m_pitch = 0.0f;
}

GameCameraControl::~GameCameraControl() {}

void GameCameraControl::onMouse(int button, int action, double xpos, double ypos)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mIsLocked = !mIsLocked;
		GLFWwindow* window = glfwGetCurrentContext();

		if (mIsLocked) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			mFirstMouse = true;
			std::cout << "鼠标已锁定 (FPS模式)" << std::endl;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			std::cout << "鼠标已解锁 (普通模式)" << std::endl;
		}
		mCurrentX = xpos;
		mCurrentY = ypos;
	}
	CameraControl::onMouse(button, action, xpos, ypos);
}

void GameCameraControl::onCursor(double xpos, double ypos)
{
	if (!mIsLocked) return;

	if (mFirstMouse) {
		mCurrentX = xpos;
		mCurrentY = ypos;
		mFirstMouse = false;
		return;
	}

	float deltaX = (xpos - mCurrentX) * mSensitivity;
	float deltaY = (ypos - mCurrentY) * mSensitivity;

	mCurrentX = xpos;
	mCurrentY = ypos;

	m_yaw += deltaX;
	m_pitch -= deltaY;

	// 限制上下看的角度（防止翻转）
	if (m_pitch > 89.0f) m_pitch = 89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;

	// 使用 Yaw 和 Pitch 计算新的相机朝向
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	// 更新相机的前向量（假设你的相机有 front 或类似变量）
	// 如果 Camera 类没有 front，我们需要通过修改 mTarget 或 mUp 来实现
	// 但看你的 update，应该是用 mUp 和 mRight 算的。
	glm::vec3 normalizedFront = glm::normalize(front);

	// 重新计算相机的 Right 和 Up
	mCamera->mRight = glm::normalize(glm::cross(normalizedFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	mCamera->mUp = glm::normalize(glm::cross(mCamera->mRight, normalizedFront));
}

void GameCameraControl::update()
{
	// 只有在锁定时才允许 WASD 移动
	if (!mIsLocked) return;

	glm::vec3 direction(0.0f);
	// 注意：这里假设你的相机的视线方向是 cross(mRight, mUp)
	glm::vec3 front = glm::cross(mCamera->mUp, mCamera->mRight);
	glm::vec3 right = mCamera->mRight;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // 使用世界坐标的上方向来升降

	if (mKeyMap[GLFW_KEY_W]) direction += front;
	if (mKeyMap[GLFW_KEY_S]) direction -= front;
	if (mKeyMap[GLFW_KEY_A]) direction -= right;
	if (mKeyMap[GLFW_KEY_D]) direction += right;
	if (mKeyMap[GLFW_KEY_SPACE]) direction += up;
	if (mKeyMap[GLFW_KEY_LEFT_SHIFT]) direction -= up;

	if (glm::length(direction) != 0)
	{
		direction = glm::normalize(direction);
		mCamera->mPosition += direction * m_wasd_speed;
	}
}