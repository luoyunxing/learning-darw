#pragma once 

#include "cameracontrol.h"

class GameCameraControl : public CameraControl
{
public:
	GameCameraControl();
	~GameCameraControl();

	void onMousePress(int button, int action, double xpos, double ypos);
	void onCursor(double xpos, double ypos)override;
	void update()override;

private:

	void pitch(float angle);
	void yaw(float angle);

private:

	float mAngleLimit{ 0.0f };
	float m_wasd_speed{ 0.1f };
};