#pragma once
#include "cameracontrol.h"

class Trackballcameracontrol : public CameraControl
{
public:

	Trackballcameracontrol();
	~Trackballcameracontrol();

	void onCursor(double xpos, double ypos)override;

private:
	//沿着经线方向的角
	void pitch(float angle);
	//沿着纬线方向的角
	void yaw(float angle);
};