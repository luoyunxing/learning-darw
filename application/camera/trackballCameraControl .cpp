#include "trackballCameraControl.h"

Trackballcameracontrol::Trackballcameracontrol()
{

}

Trackballcameracontrol::~Trackballcameracontrol()
{

}



void Trackballcameracontrol::onCursor(double xpos, double ypos)
{
	if (mLeftMouseDown)
	{
		//更新变换坐标
		float deltaX = (xpos - mCurrentX) * mSensitivity;
		float deltaY = (ypos - mCurrentY) * mSensitivity;

		//分开计算pitch 和 yaw 
		pitch(-deltaY);
		yaw(-deltaX);
	}
	else if (mMiddleMouseDown)
	{
		//更新变换坐标
		float deltaX = (xpos - mCurrentX) * mMoveSpeed;
		float deltaY = (ypos - mCurrentY) * mMoveSpeed;

		mCamera->mPosition -= mCamera->mRight * deltaX;
		mCamera->mPosition += mCamera->mUp * deltaY;
	}

	mCurrentX = xpos;
	mCurrentY = ypos;
}

void Trackballcameracontrol::pitch(float angle)
{
	//绕着mRight向量在旋转
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

	//影响当前相机的up向量和position                /*  三维数据升级四维数据 ：0.0f 表示向量  1.0f 表示点  */
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void Trackballcameracontrol::yaw(float angle)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f , 1.0f , 0.0f));
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}