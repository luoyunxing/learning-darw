#pragma once
#include "light.h"

class PointLight : public Light
{
public:
    PointLight() {
        mLightType = LightType::Point; // 自动标识为点光源

        // 点光源特有的默认衰减参数
        mConstant = 1.0f;
        mLinear = 0.09f;
        mQuadratic = 0.032f;
    }
    ~PointLight();


    // 衰减系数
    float mConstant;
    float mLinear;
    float mQuadratic;
};