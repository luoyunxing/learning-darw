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
	Scene* scene,
	Camera* camera,
	const std::vector<Light::LightData>& lights,
	AmbientLight* ambientLight
)
{
	//首先设置OpenGL状态机必要参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//画布清理
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// 从场景的根节点开始递归渲染
	renderNode(scene, camera, lights, ambientLight);
}


void Render::renderNode(
	Object* node,
	Camera* camera,
	const std::vector<Light::LightData>& lights,              // 统一
	AmbientLight* ambientLight
)
{
	//不为挂载（空）节点
	if (!node) return;


	//使用类型枚举判断，替代 dynamic_cast
	if (node->mType == ObjectType::Mesh)
	{
		// 既然已经确定了它是 Mesh，就可以安全地进行静态转换（static_cast 几乎不消耗性能）
		Mesh* mesh = static_cast<Mesh*>(node);
		if (mesh->mMeterial)
		{
			Geometry* geometry = mesh->mGemetry;
			Meterial* meterial = mesh->mMeterial;
			//判断mesh材质类型
			Shader* shader = pickShader(meterial->mType);

			if (shader)
			{
				//更新shader uniform变量
				shader->begin();

				switch (meterial->mType)
				{
				case MeterialType::PhoneMeterial:
				{
					PhoneMeterial* PhoneMat = (PhoneMeterial*)meterial;
					shader->setInt("sampler", 0);
					if (PhoneMat->mDiffuse) PhoneMat->mDiffuse->bind();

					shader->setInt("numLights", lights.size());
					//点光源，探照灯
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
					if (ambientLight) {
						shader->setVector3("AmbientColor", ambientLight->mAmbientColor);
						shader->setFloat("Instensity", ambientLight->mSpecularIntensity);
						shader->setFloat("SpotSize", ambientLight->mSpotSize);
						shader->setFloat("LightEdge", ambientLight->mLightEdge);
						shader->setFloat("FresnelPower", ambientLight->mFresnelPower);
					}
					//模型矩阵
					shader->setMatrix44("modelMatrix", mesh->getmodelMatrix());
					//法线矩阵
					glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mesh->getmodelMatrix())));
					shader->setMatrix33("normalMatrix", normalMatrix);
				}
				break;
				//白色灯泡颜色参数
				case MeterialType::WhiteMeterial:
					shader->setVector3("ObjectColor", meterial->mColor);
					//模型矩阵
					shader->setMatrix44("modelMatrix", mesh->getmodelMatrix());
					break;
					//青色灯泡参数
				case MeterialType::CyanMeterial:
					shader->setVector3("ObjectColor", meterial->mColor);
					//模型矩阵
					shader->setMatrix44("modelMatrix", mesh->getmodelMatrix());
					break;
				}

				//相机参数统一上传
				shader->setVector3("CameraPosition", camera->mPosition);
				shader->setMatrix44("viewMatrix", camera->getViewMatrix());
				shader->setMatrix44("projectionMatrix", camera->getprojectionMatrix());

				// 绘制
				GL_CALL(glBindVertexArray(geometry->getVao()));
				glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
			}
		}
	}

	//递归查找子节点
	for (Object* child : node->getChildren())
	{
		renderNode(child, camera, lights, ambientLight);
	}
}