#pragma once

#include "meterial.h"
#include "../texture.h"

class PhoneMeterial : public Meterial
{
public:

	PhoneMeterial();
	~PhoneMeterial();

public:
	//meterial就是物体表面的材质
	//贴图  ， 物体表面的高光


	//贴图
	Texture* mDiffuse{ nullptr };
};