#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

enum class LightType
{
	Directional = 0, // 平行光
	Point = 1,       // 点光源
	Spot = 2         // 探照灯
};

class Light
{
public:
	Light();
	virtual ~Light();
public:
	// 当前灯光的类型（子类构造时需指定）
	LightType mLightType{};

    // 这个结构体用于在 C++ 和 Shader 之间传递统一的灯光数据
    struct LightData {
        int type;           // 灯光类型
        glm::vec3 position; // 位置 (平行光忽略)
        glm::vec3 direction;// 方向 (平行光/探照灯使用)
        glm::vec3 color;    // 颜色

        // 衰减
        float constant;
        float linear;
        float quadratic;

        // 探照灯角度
        float cutOff;
        float outerCutOff;
    };
};