#pragma once
#include "light.h"

class DirectionLight : public Light
{
public:
	DirectionLight();
	~DirectionLight();

public:
	glm::vec3 mDirection{ -1.0f };
};