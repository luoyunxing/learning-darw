#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:

	Camera();
	~Camera();

	//获取摄像机矩阵
	glm::mat4 getViewMatrix();

	//获取正交投影与透视投影矩阵
	virtual glm::mat4 getprojectionMatrix();

public:

	//摄像机位置
	glm::vec3 mPosition{ 0.0f , 0.0f , 6.0f };
	
	//摄像机up向量
	glm::vec3 mUp{ 0.0f , 1.0f , 0.0f };

	//摄像机X轴方向
	glm::vec3 mRight{ 1.0f , 0.0f , 0.0f };
};
