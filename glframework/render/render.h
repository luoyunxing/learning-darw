#pragma once

#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionlight.h"
#include "../light/ambientlight.h"
#include "../meterial/phonemeterial.h"
#include "../shader.h"
#include "../light/PointLight.h"
#include "../light/spotLight.h"
#include "../light/light.h"
#include <vector>



class Render
{
public:
	Render();
	~Render();

	//渲染所需参数
	//每次调用render渲染一帧
	void render(
		const std::vector<Mesh*>& meshes,
		Camera* camera,
		DirectionLight* dirLight,
		AmbientLight* ambientLight,
		PointLight* pointLight
		);

	void render1(
		const std::vector<Mesh*>& meshes,
		Camera* camera,
		Light* Light,
		AmbientLight* ambientLight
		);

private:
	//根据meterial类型的不同，选不同的shader
	Shader* pickShader(MeterialType type);

private:
	//根据材质类型的选择不同shader
	Shader* mPhongShader{ nullptr };
	Shader* mWhiteShader{ nullptr };
};