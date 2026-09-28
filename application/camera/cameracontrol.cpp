#include "cameracontrol.h"
#include <iostream>

CameraControl::CameraControl()
{

}
CameraControl::~CameraControl()
{

}

//鼠标按键控制
void CameraControl::onMouse(int button, int action, double xpos, double ypos)
{
	//std::cout << "onMouse" << std::endl;
	//按键是否按下
	bool pressed = action == GLFW_PRESS ? true : false;

	//按下则记录
	if (pressed)
	{
		mCurrentX = xpos;
		mCurrentY = ypos;
	}

	//根据按下鼠标按间不同，更改按键状态
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}
}

void CameraControl::onCursor(double xpos, double ypos)
{
	//std::cout << "onCursor" << std::endl;
}

void CameraControl::onKey(int key, int action, int mods)
{
	//std::cout << "onkey" << std::endl;
	//过滤掉repeat情况
	if (action == GLFW_REPEAT)
	{
		return;
	}
	//按键是否按下
	bool pressed = action == GLFW_PRESS ? true : false;

	//记录在keyMap
	mKeyMap[key] = pressed;
}

void CameraControl::update()
{

}



