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

void Trackballcameracontrol::onScroll(double offset)
{
	if (mCamera == nullptr) return;

	// 缩放灵敏度，控制滚轮一次移动的距离
	float scrollSpeed = 0.5f;

	// 获取相机看向的方向向量 (视线的反方向)
	// 假设你有 mRight 和 mUp，那么前方向可以通过叉乘计算出来：
	// 注意：根据你的坐标系（右手系/左手系），可能需要调整正负号
	glm::vec3 forward = glm::cross(mCamera->mRight, mCamera->mUp);

	// 防止 forward 受到浮点数精度影响失去单位长度
	forward = glm::normalize(forward);

	// 滚轮向上滚动(offset > 0)，相机向前移动（拉近）
	// 滚轮向下滚动(offset < 0)，相机向后移动（推远）
	mCamera->mPosition += forward * float(offset) * scrollSpeed;
}