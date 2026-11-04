#pragma once

#include "meterial.h"
#include "../texture.h"

class PhoneMeterial : public Meterial
{
public:

	PhoneMeterial();
	~PhoneMeterial();

public:
	Texture* mDiffuse{ nullptr };
	float mShiness{ 1.0f };
};