#pragma once
#include "light.h"

class AmbientLight : public Light
{
public:
	AmbientLight();
	~AmbientLight();
public:
	glm::vec3 mAmbientColor{ 0.3f,0.3f, 0.3f };
	glm::float32 mSpotSize{ 32.0f };
	glm::float32 mLightEdge{ 0.6f };
	glm::float32 mFresnelPower{ 1.0f };

	//float mSpecularIntensity{ 0.05f };
};