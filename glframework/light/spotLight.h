#pragma once
#include "light.h"

class SpotLight : public Light
{
public:
	SpotLight() {
		mLightType = LightType::Spot; // 自动标识为探照灯
		mDirection = glm::vec3(0.0f, -1.0f, 0.0f);
		mCutOff = glm::cos(glm::radians(12.5f));
		mOuterCutOff = glm::cos(glm::radians(17.5f));
		mConstant = 1.0f;
		mLinear = 0.09f;
		mQuadratic = 0.032f;
	}
	~SpotLight() {}

	glm::vec3 mDirection;
	float mCutOff;
	float mOuterCutOff;
	float mConstant;
	float mLinear;
	float mQuadratic;
};