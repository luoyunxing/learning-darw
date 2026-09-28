#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include <map>
#include <GLFW/glfw3.h>

class CameraControl
{
public:
	CameraControl();
	~CameraControl();

	//鼠标按键控制
	virtual void onMouse(int button, int action, double xpos, double ypos);
	virtual void onCursor(double xpos, double ypos);
	virtual void onKey(int key, int action, int mods);

	virtual void update();
	
	//绑定相机
	void setCamera(Camera* camera)
	{
		mCamera = camera;
	}

	//设置灵敏度
	void setSensitivitity(float s)
	{
		mSensitivity = s;
	}

protected:
	//鼠标按键状态
	bool mLeftMouseDown = false;
	bool mRightMouseDown = false;
	bool mMiddleMouseDown = false;

	//当前鼠标状态
	float mCurrentX = 0.0f, mCurrentY = 0.0f;

	//鼠标灵敏度
	float mSensitivity = 0.2f;
	//平移灵敏度
	float mMoveSpeed = 0.01f;

	//记录键盘按下相关状态
	std::map<int, bool > mKeyMap;

	//存储当前控制的摄像机
	Camera* mCamera = nullptr;
};