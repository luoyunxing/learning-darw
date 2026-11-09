#include "render.h"

Render::Render()
{
	mPhongShader = new Shader(
		"assets/shaders/vertex.glsl",
		"assets/shaders/fragment.glsl"
	);
}


Render::~Render()
{

}

void Render::render(
	const std::vector<Mesh*>& meshes,
	Camera* camera,
	DirectionLight* dirLight,
	AmbientLight* ambientLight
)
{
	//首先设置OpenGL状态机必要参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//画布清理
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//遍历mesh执行绘制
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = meshes[i];
		Geometry* geometry = mesh->mGemetry;
		Meterial* meterial = mesh->mMeterial;

		//决定使用哪个shader
		Shader* shader = pickShader(meterial->mType);

		//更新该shader的uniform变量
		shader->begin();

		switch (meterial->mType)
		{
			case MeterialType::PhoneMeterial:
			{
				//强制类型转换
				PhoneMeterial* PhoneMat = (PhoneMeterial*)meterial;

				//将采样器与纹理单元挂钩
				//shader->setInt("sampler", 0);
				shader->setInt("earth_sampler", 0);
				
				//将纹理与纹理单元挂钩
				PhoneMat->mDiffuse->bind();

				//光
				shader->setVector3("LightDirection", dirLight->mDirection);
				shader->setVector3("LightColor", dirLight->mColor);
				shader->setVector3("AmbientColor", ambientLight->mAmbientColor);
				shader->setFloat("Instensity", ambientLight->mSpecularIntensity);
				shader->setFloat("SpotSize", ambientLight->mSpotSize);
				shader->setFloat("LightEdge", ambientLight->mLightEdge);
				shader->setFloat("FresnelPower", ambientLight->mFresnelPower);

				//相机矩阵
				shader->setVector3("CameraPosition", camera->mPosition); //相机位置
				shader->setMatrix44("viewMatrix", camera->getViewMatrix());
				shader->setMatrix44("projectionMatrix", camera->getprojectionMatrix());
				//模型矩阵
				shader->setMatrix44("modelMatrix", mesh->getmodelMatrix());
				//法线矩阵
				glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mesh->getmodelMatrix())));
				shader->setMatrix33("normalMatrix", normalMatrix);

			}
				break;
			default:
				continue;
		}

		//绑定到vao
		GL_CALL(glBindVertexArray(geometry->getVao()));
		//执行绘制
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}

}

Shader* Render::pickShader(MeterialType type)
{
	Shader* result = nullptr;

	switch (type) 
	{
	case MeterialType::PhoneMeterial:
		result = mPhongShader;
		break;
	default:
		break;
	}
	return result;
}