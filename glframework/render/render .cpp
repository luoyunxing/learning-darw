#include "render.h"

Render::Render()
{
	//mPhongShader = new Shader(
	//	"assets/shaders/phone.vert",
	//	"assets/shaders/phone.frag"
	//);
	mWhiteShader = new Shader(
		"assets/shaders/white.vert",
		"assets/shaders/white.frag"
	);
	//点光源phone材质shader
	mPhongShader = new Shader(
		"assets/shaders/point.vert",
		"assets/shaders/point.frag"
	);
	
}


Render::~Render()
{

}

void Render::render(
	const std::vector<Mesh*>& meshes,
	Camera* camera,
	DirectionLight* dirLight,
	AmbientLight* ambientLight,
	PointLight* pointLight
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
				shader->setInt("sampler", 0);
				
				//将纹理与纹理单元挂钩
				PhoneMat->mDiffuse->bind();

				//平行光
				shader->setVector3("LightDirection", dirLight->mDirection);
				shader->setVector3("LightColor", dirLight->mColor);

				//点光源
				shader->setVector3("LightPosition", pointLight->mPosition); // 位置
				shader->setVector3("LightColor", pointLight->mColor);       // 颜色

				// 距离衰减参数
				shader->setFloat("Constant", pointLight->mConstant);
				shader->setFloat("Linear", pointLight->mLinear);
				shader->setFloat("Quadratic", pointLight->mQuadratic);

				//菲涅尔高光参数
				shader->setVector3("AmbientColor", ambientLight->mAmbientColor);
				shader->setFloat("Instensity", ambientLight->mSpecularIntensity);
				shader->setFloat("SpotSize", ambientLight->mSpotSize);
				shader->setFloat("LightEdge", ambientLight->mLightEdge);
				shader->setFloat("FresnelPower", ambientLight->mFresnelPower);

				//环境光参数
				if (ambientLight != nullptr) {
					shader->setVector3("AmbientColor", ambientLight->mAmbientColor);
					shader->setFloat("Instensity", ambientLight->mSpecularIntensity);
					shader->setFloat("SpotSize", ambientLight->mSpotSize);
					shader->setFloat("LightEdge", ambientLight->mLightEdge);
					shader->setFloat("FresnelPower", ambientLight->mFresnelPower);
				}

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
				//相机矩阵
				shader->setVector3("CameraPosition", camera->mPosition); //相机位置
				shader->setMatrix44("viewMatrix", camera->getViewMatrix());
				shader->setMatrix44("projectionMatrix", camera->getprojectionMatrix());
				//模型矩阵
				shader->setMatrix44("modelMatrix", mesh->getmodelMatrix());
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
	case MeterialType::WhiteMeterial:
		result = mWhiteShader;
		break;
	default:
		break;
	}
	return result;
}


void Render::render1(
	const std::vector<Mesh*>& meshes,
	Camera* camera,
	Light* light,               // 统一的灯指针
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

			shader->setInt("LightType", (int)light->mLightType);
			shader->setVector3("LightColor", light->mColor);
			shader->setVector3("LightPosition", light->mPosition);

			//根据具体的灯光类型，去强转子类，把特有的属性传给 Shader
			//平行光uniform变量
			if (light->mLightType == LightType::Directional)
			{
				DirectionLight* dir = dynamic_cast<DirectionLight*>(light);
				if (dir) {
					shader->setVector3("LightDirection", dir->mDirection);
				}
			}

			//点光源uniform变量
			else if (light->mLightType == LightType::Point)
			{
				PointLight* point = dynamic_cast<PointLight*>(light);
				if (point) {
					shader->setFloat("Constant", point->mConstant);
					shader->setFloat("Linear", point->mLinear);
					shader->setFloat("Quadratic", point->mQuadratic);
				}
			}

			//探照灯uniform变量
			else if (light->mLightType == LightType::Spot)
			{
				SpotLight* spot = dynamic_cast<SpotLight*>(light);
				if (spot) {
					shader->setVector3("LightDirection", glm::normalize(spot->mDirection));
					shader->setFloat("CutOff", spot->mCutOff);
					shader->setFloat("OuterCutOff", spot->mOuterCutOff);
					shader->setFloat("Constant", spot->mConstant);
					shader->setFloat("Linear", spot->mLinear);
					shader->setFloat("Quadratic", spot->mQuadratic);
				}
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
