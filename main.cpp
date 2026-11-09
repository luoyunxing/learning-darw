#include "wrapper/OnOff.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/application.h"
#include "glframework/shader.h"
#include "glframework/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "application/camera/camera.h"
#include "application/camera/perspectivegraphicCamera.h"
#include "application/camera/trackballCameraControl.h"
#include "application/camera/gameCameraControl.h"
#include "glframework/geometry.h"
#include "glframework/meterial/phonemeterial.h"
#include "glframework/object.h"
#include "glframework/mesh.h"
#include "glframework/render/render.h"

Render* render = nullptr;
Geometry* geometry = nullptr;
DirectionLight* dirLight = nullptr;
AmbientLight* ambientLight = nullptr;
std::vector<Mesh*> meshes{};
perspectivegraphicCamera* camera = nullptr;
//轨迹球相机
#if !ENABLE_CAMERA_CODE
Trackballcameracontrol* cameraControl = nullptr;
#endif
//游戏相机
#if ENABLE_CAMERA_CODE
GameCameraControl* cameraControl = nullptr;
#endif

// ██████████████████████████████████████████████████████████████
// █                        这些已经没用了，到后面直接用mgui                         █
// ██████████████████████████████████████████████████████████████
glm::vec3 LightDirection = glm::vec3(-1.0f, -1.0f, -1.0f);  // 平行光方向
glm::vec3 LightColor = glm::vec3(1.0f, 0.98f, 0.96f);   // 主光
glm::vec3 AmbientColor = glm::vec3(0.3f, 0.3f, 0.32f);    // 环境光
glm::float32 SpotSize = 32.0f;                           // 光斑大小(1~128) 越大光斑越小
glm::float32 Instensity = 0.005f;                          // 光强度
glm::float32 LightEdge = 0.6f;                            // 边缘衰减
glm::float32 FresnelPower = 1.0f;                            // 菲涅尔过渡 1柔和→8锐利
//设置旋转

// ██████████████████████████████████████████████████████████████

//视口大小回调函数
void onResize(int width, int height)
{
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "on" << std::endl;
}

//键盘回调函数
void keyCallback(int key, int action, int mods)
{
	//std::cout << key << std::endl;
	cameraControl->onKey(key, action, mods);
}

//鼠标按键回调函数
void mouseCallback(int button, int action, int mod)
{
	//std::cout << button << std::endl;

	double x, y;
	Application::getInstance()->getCursorPosition(&x, &y);
	cameraControl->onMouse(button, action, x, y);
}

//光标位置回调函数
void cursorCallback(double xpos, double ypos)
{
	//std::cout << xpos <<   "  " << ypos << std::endl;
	cameraControl->onCursor(xpos, ypos);
}

//滚轮回调函数
void scrollCallback(double xoffset, double yoffset)
{
	// 将滚轮偏移量传给 cameraControl
	cameraControl->onScroll(yoffset);
}

void prepare()
{
	render = new Render();
	//创建几何体
	geometry = Geometry::createSphere(3.0f);
	//创建材质
	PhoneMeterial* meterial = new PhoneMeterial();
	meterial->mDiffuse = new Texture("assets/texture/earth.png", 0);

	//装入mesh等待渲染
	Mesh* mesh = new Mesh(geometry, meterial);
	meshes.push_back(mesh);
	dirLight = new DirectionLight();
	ambientLight = new AmbientLight();
}

//设置摄像机矩阵
void prepareCamera()
{
	float aspect = static_cast<float>(Application::getInstance()->getWidth()) / static_cast<float>(Application::getInstance()->getHeight());
	camera = new perspectivegraphicCamera(60.0f, aspect, 0.1f, 1000.0f);

	//决定初始位置
	camera->mPosition = glm::vec3(0.0f, 0.0f, 6.0f);

#if !ENABLE_CAMERA_CODE
	cameraControl = new Trackballcameracontrol();
#endif

#if ENABLE_CAMERA_CODE
	cameraControl = new GameCameraControl();
#endif

	cameraControl->setCamera(camera);

};

int main()
{
	//初始化
	if (!Application::getInstance()->init(800, 600))
	{
		return -1;
	}

	//传入视口大小回调函数
	Application::getInstance()->setResizeCallback(onResize);

	//键盘输入回调函数
	Application::getInstance()->setKeyBoradCallback(keyCallback);

	//鼠标按键回调函数
	Application::getInstance()->setMouseCallback(mouseCallback);

	//光标位置回调函数
	Application::getInstance()->setCursorCallback(cursorCallback);

	//鼠标滚轮回调函数
	Application::getInstance()->setScrollCallback(scrollCallback);

	//设置OpenGL视口
	GL_CALL(glViewport(0, 0, 800, 600));

	//画布清理颜色
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 0.1f));
	
	//设置摄像机矩阵
	prepareCamera();

	//
	prepare();

	//执行窗体循环
	while (Application::getInstance()->update())
	{
		//模型变换
		float angleDegrees = 0.1f;
		glm::vec3 axis = glm::vec3(-1.0, 1.0, 1.0);
		meshes[0]->rotateArbitraryAxis(angleDegrees, axis);

		cameraControl->update();
		//绘制
		render->render(meshes, camera, dirLight, ambientLight);
	}

	Application::getInstance()->destroy();

	return 0;
}