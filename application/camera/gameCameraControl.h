#pragma once 
#include "cameracontrol.h"

class GameCameraControl : public CameraControl
{
public:
	GameCameraControl();
	~GameCameraControl();

	void onMouse(int button, int action, double xpos, double ypos) override;
	void onCursor(double xpos, double ypos) override;
	void update() override;

private:
	float mAngleLimit{ 0.0f };
	float m_wasd_speed{ 0.1f };

	bool mIsLocked = false;
	bool mFirstMouse = true;

	// 新增：存储当前的偏航角和俯仰角
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;
};