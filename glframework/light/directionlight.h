#pragma once
#include "light.h"

class DirectionLight : public Light
{
public:

	DirectionLight() {
		mLightType = LightType::Directional; // 自动标识为平行光
	}

	~DirectionLight();

public:
	glm::vec3 mDirection{ -1.0f };

	glm::vec3 mColor{ 1.0f, 1.0f, 1.0f };
};