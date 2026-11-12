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
#include "glframework/meterial/whitemeterial.h"
#include "glframework/object.h"
#include "glframework/mesh.h"
#include "glframework/render/render.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/light.h"
#include "glframework/light/spotLight.h"
#include "glframework/meterial/cyanmeterial.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

std::vector <Light::LightData> sceneLights;
Light* currentLight = nullptr;
Render* render = nullptr;
AmbientLight* ambientLight = nullptr;
std::vector<Mesh*> meshes{};
perspectivegraphicCamera* camera = nullptr;

//灯光调色
glm::vec3 whiteColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 cyanColor = glm::vec3(0.0f, 1.0f, 1.0f);
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
	Geometry* geometry = nullptr;
	Geometry* geometryBox = nullptr;
	render = new Render();

	// ==========================================
	// 1. 创建几何体和材质（这部分保持不变）
	// ==========================================
	geometry = Geometry::createSphere(3.0f);
	geometryBox = Geometry::createBox(6.0f);

	// 材质1
	PhoneMeterial* meterial01 = new PhoneMeterial();
	meterial01->mDiffuse = new Texture("assets/texture/earth.png", 0);

	// 材质2
	PhoneMeterial* meterial02 = new PhoneMeterial();
	meterial02->mDiffuse = new Texture("assets/texture/xiaojuzhihua.png", 0);

	// 装入mesh等待渲染
	Mesh* mesh01 = new Mesh(geometry, meterial01);
	Mesh* mesh02 = new Mesh(geometryBox, meterial02);

	meshes.push_back(mesh01);
	meshes.push_back(mesh02);

	// 创建白色材质 (太阳/灯泡)
	Geometry* geometrySun = nullptr;
	geometrySun = Geometry::createSphere(0.5f);
	WhiteMeterial* meterial03 = new WhiteMeterial();
	//材质颜色
	meterial03->mColor = whiteColor;  
	Mesh* mesh03 = new Mesh(geometrySun, meterial03);
	mesh03->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));//点光源位置
	meshes.push_back(mesh03);

	// 创建青色材质 (太阳/灯泡)
	Geometry* geometrySpot = nullptr;
	geometrySpot = Geometry::createSphere(0.2f);
	CyanMaterial* meterial04 = new CyanMaterial();
	//材质颜色
	meterial04->mColor = cyanColor;
	Mesh* mesh04 = new Mesh(geometrySpot, meterial04);
	//探照灯光源位置
	mesh04->setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
	meshes.push_back(mesh04);

	// 1. 添加一个平行光（比如太阳光）
	Light::LightData dirLight;
	dirLight.type = 0;
	dirLight.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	dirLight.color = glm::vec3(0.3f, 0.3f, 0.3f); // 稍微弱一点
	sceneLights.push_back(dirLight);
	
	// 2. 添加一个点光源（比如在原点上方）
	Light::LightData pointLight;
	pointLight.type = 1;
	pointLight.position = mesh03->getPosition();
	pointLight.color = whiteColor;
	pointLight.constant = 1.0f;
	pointLight.linear = 0.09f;
	pointLight.quadratic = 0.032f;
	sceneLights.push_back(pointLight);

	// 3. 添加一个探照灯（比如从侧面照射）
	Light::LightData spotLight;
	spotLight.type = 2;
	spotLight.position = mesh04->getPosition();
	spotLight.direction = glm::normalize(glm::vec3(-5.0f, -5.0f, -5.0f));
	spotLight.color = cyanColor; // 青色光
	spotLight.constant = 1.0f;
	spotLight.linear = 0.09f;
	spotLight.quadratic = 0.032f;
	spotLight.cutOff = glm::cos(glm::radians(12.5f));
	spotLight.outerCutOff = glm::cos(glm::radians(17.5f));
	sceneLights.push_back(spotLight);

	//环境光
	ambientLight = new AmbientLight();
	ambientLight->mAmbientColor = glm::vec3(0.2f, 0.2f, 0.2f); 
	ambientLight->mSpecularIntensity = 0.5f;
	ambientLight->mSpotSize = 32.0f;
	ambientLight->mLightEdge = 0.2f;
	ambientLight->mFresnelPower = 5.0f;
}

void setColor()
{
	if (meshes.size() > 2) {
		WhiteMeterial* whiteMat = static_cast<WhiteMeterial*>(meshes[2]->mMeterial);
		whiteMat->mColor = whiteColor;
	}

	if (meshes.size() > 3) {
		CyanMaterial* cyanMat = static_cast<CyanMaterial*>(meshes[3]->mMeterial);
		cyanMat->mColor = cyanColor;
	}

	//这里0是平行光    1 是点光源      2 是探照灯
	if (sceneLights.size() > 1) {
		sceneLights[1].color = whiteColor; // 更新点光源颜色
	}
	if (sceneLights.size() > 2) {
		sceneLights[2].color = cyanColor;  // 更新探照灯颜色
	}
}

//初始化imgui
void InitImgui()
{
	ImGui::CreateContext(); //创建imgui上下文
	ImGui::StyleColorsDark();//选择黑色主题

	//设置imgui与glfw和OpenGL的绑定
	ImGui_ImplGlfw_InitForOpenGL(Application::getInstance()->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

//绘制ui层
void renderIMGUI()
{
	//开启当前IMGUI渲染
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//决定当前的GUI上面有哪些控件，从上到下
	ImGui::Begin("Hello,world!");

	// 调色盘控件
	ImGui::Text("White Ball Color");
	if (ImGui::ColorEdit3("White Color", (float*)&whiteColor)) {
		setColor(); // 颜色变化时实时更新材质
	}

	ImGui::Spacing();

	ImGui::Text("Cyan Ball Color");
	if (ImGui::ColorEdit3("Cyan Color", (float*)&cyanColor)) {
		setColor(); // 颜色变化时实时更新材质
	}

	ImGui::End();

	//执行ui渲染
	ImGui::Render();
	//获取当前窗口的宽高
	int display_w, display_h;
	glfwGetFramebufferSize(Application::getInstance()->getWindow(), &display_w, &display_h);
	//重置视口大小
	glViewport(0, 0, display_w, display_h);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void dotransform()
{
	//模型变换
	float angleDegrees = 0.1f;
	glm::vec3 axis = glm::vec3(-1.0, 1.0, 1.0);
	meshes[0]->rotateArbitraryAxis(angleDegrees, axis);

	meshes[1]->setPosition(glm::vec3(8.0f, 0.0f, 0.0f));
}

//设置摄像机矩阵
void prepareCamera()
{
	float aspect = static_cast<float>(Application::getInstance()->getWidth()) / static_cast<float>(Application::getInstance()->getHeight());
	camera = new perspectivegraphicCamera(60.0f, aspect, 0.1f, 1000.0f);

	//决定初始位置
	camera->mPosition = glm::vec3(0.0f, 0.0f, 12.0f);

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
	if (!Application::getInstance()->init(1600, 1200))
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
	GL_CALL(glViewport(0, 0, 1600, 1200));

	//画布清理颜色
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 0.1f));
	
	//设置摄像机矩阵
	prepareCamera();

	InitImgui();

	//
	prepare();

	//执行窗体循环
	while (Application::getInstance()->update())
	{
		setColor();
		dotransform();
		cameraControl->update();
		render->render(meshes, camera, sceneLights, ambientLight);
		renderIMGUI();
	}

	Application::getInstance()->destroy();

	return 0;
}