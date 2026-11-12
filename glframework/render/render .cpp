#include "render.h"

Render::Render()
{
	mPhongShader = new Shader(
		"assets/shaders/phone.vert",
		"assets/shaders/phone.frag"
	);
	mWhiteShader = new Shader(
		"assets/shaders/white.vert",
		"assets/shaders/white.frag"
	);
	mCyanShader = new Shader(
		"assets/shaders/cyan.vert",
		"assets/shaders/cyan.frag"
	);
}


Render::~Render()
{

}


Shader* Render::pickShader(MeterialType type)
{
	Shader* result = nullptr;

	switch (type) 
	{
	case MeterialType::PhoneMeterial:
		result = mPhongShader;
		break;
	case MeterialType::WhiteMeterial:
		result = mWhiteShader;
		break;
	case MeterialType::CyanMeterial:
		result = mCyanShader;
		break;
	default:
		break;
	}
	return result;
}


void Render::render(
	const std::vector<Mesh*>& meshes,
	Camera* camera,
	const std::vector<Light::LightData>& lights,              // 统一
	AmbientLight* ambientLight
)
{//首先设置OpenGL状态机必要参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//画布清理
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//遍历mesh进行绘制
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = meshes[i];
		Geometry* geometry = mesh->mGemetry;
		Meterial* meterial = mesh->mMeterial;

		//决定使用那个shader
		Shader* shader = pickShader(meterial->mType);
		if (!shader) continue;

		//更新该shader的uniform变量
		shader->begin();

		//判断mesh材质类型
		switch (meterial->mType)
		{
		case MeterialType::PhoneMeterial:
		{
			PhoneMeterial* PhoneMat = (PhoneMeterial*)meterial;
			shader->setInt("sampler", 0);
			if (PhoneMat->mDiffuse) PhoneMat->mDiffuse->bind();

			// ==========================================
			// 统一处理灯光传参
			// ==========================================
			shader->setInt("numLights", lights.size());

			for (int j = 0; j < lights.size(); j++) 
			{
				std::string prefix = "lights[" + std::to_string(j) + "].";
				shader->setInt(prefix + "type", lights[j].type);
				shader->setVector3(prefix + "position", lights[j].position);
				shader->setVector3(prefix + "direction", lights[j].direction);
				shader->setVector3(prefix + "color", lights[j].color);
				shader->setFloat(prefix + "constant", lights[j].constant);
				shader->setFloat(prefix + "linear", lights[j].linear);
				shader->setFloat(prefix + "quadratic", lights[j].quadratic);
				shader->setFloat(prefix + "cutOff", lights[j].cutOff);
				shader->setFloat(prefix + "outerCutOff", lights[j].outerCutOff);
			}

			//菲涅尔高光参数
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
		case MeterialType::WhiteMeterial:
		{
			shader->setVector3("CameraPosition", camera->mPosition);
			shader->setMatrix44("viewMatrix", camera->getViewMatrix());
			shader->setMatrix44("projectionMatrix", camera->getprojectionMatrix());
			shader->setMatrix44("modelMatrix", mesh->getmodelMatrix());
			shader->setVector3("ObjectColor", meterial->mColor);
		}
		break;
		case MeterialType::CyanMeterial:
		{
			shader->setVector3("CameraPosition", camera->mPosition);
			shader->setMatrix44("viewMatrix", camera->getViewMatrix());
			shader->setMatrix44("projectionMatrix", camera->getprojectionMatrix());
			shader->setMatrix44("modelMatrix", mesh->getmodelMatrix());
			shader->setVector3("ObjectColor", meterial->mColor);
		}
		break;
		default:
			continue;
		}

		//绑定vao
		GL_CALL(glBindVertexArray(geometry->getVao()));
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}
