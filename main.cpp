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

Geometry* geometry = nullptr;
Texture* nanali_texture = nullptr;
Texture* xiaoju_texture = nullptr;
Texture* earth_texture = nullptr;
Shader* shader = nullptr;

//初始模型矩阵
glm::mat4 modelMatrix = glm::mat4(1.0f);

perspectivegraphicCamera* camera = nullptr;
//轨迹球相机
#if !ENABLE_CAMERA_CODE
Trackballcameracontrol* cameraControl = nullptr;
#endif
//游戏相机
#if ENABLE_CAMERA_CODE
GameCameraControl* cameraControl = nullptr;
#endif

//光
glm::vec3 LightDirection = glm::vec3(-0.8f, -1.0f, -1.0f);  //平行光
glm::vec3 LightColor = glm::vec3(1.0f, 0.98f, 0.96f);
glm::vec3 AmbientColor = glm::vec3(0.3f, 0.3f, 0.32f);     //环境光

glm::float32  SpotSize = 32.0f;      //光斑大小（1-128）值越大光斑越小
glm::float32  Instensity = 0.005f;   //光的强度（0.1-1.0）正相关
glm::float32  LightEdge = 0.6f;      //光边缘衰减（0.2-1.0）
glm::float32  FresnelPower = 1.0f;   //光收边过度（1-8）柔和->突然

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

void scrollCallback(double xoffset, double yoffset)
{
	// 将滚轮偏移量传给 cameraControl
	cameraControl->onScroll(yoffset);
}

//创建着色器
void prepareShader()
{
	shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

//创建的vao
void prepareVao()
{
	geometry = geometry->createSphere(3.0f );
}

//渲染操作
void render()
{
	//画布清理
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//绑定到当前的program
	shader->begin();

	//更新uniform变量 time
	shader->setFloat("time", glfwGetTime());
	shader->setFloat("speed", 10.0);
	shader->setColor("uColor", 1, 1, 1);

	//纹理
	shader->setInt("nanali_sampler", 0);
	shader->setInt("xiaoju_sampler", 1);
	shader->setInt("earth_sampler", 2);

	//光
	shader->setVector3("LightDirection", LightDirection);
	shader->setVector3("LightColor", LightColor);
	shader->setVector3("AmbientColor", AmbientColor);
	shader->setFloat("SpotSize", SpotSize);
	shader->setFloat("Instensity", Instensity);
	shader->setFloat("LightEdge", LightEdge);
	shader->setFloat("FresnelPower", FresnelPower);

	//相机矩阵
	shader->setMatrix44("modelMatrix", modelMatrix);
	shader->setVector3("CameraPosition", camera->mPosition); //相机位置
	shader->setMatrix44("viewMatrix", camera->getViewMatrix());
	shader->setMatrix44("projectionMatrix",camera->getprojectionMatrix());


	//绑定到当前的vao
	GL_CALL(glBindVertexArray(geometry->getVao()));

	//执行绘制
	glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	
	//解绑vao
	GL_CALL(glBindVertexArray(0));

	//解绑program
	shader->end();
}	

//纹理读取
void prepareTexture()
{
	std::string path = "assets/texture/xiaojuzhihua.png";
	xiaoju_texture = new Texture(path.c_str(), 1);
	nanali_texture = new Texture("assets/texture/nanali.png", 0);
	earth_texture = new Texture("assets/texture/earth.png", 2);
}
//深度测试
void perpareState()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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

	//创建vao
	prepareVao();

	//深度测试
	perpareState();

	//创建Shader
	prepareShader();

	//读取纹理
	prepareTexture();
	
	//设置摄像机矩阵
	prepareCamera();

	//执行窗体循环
	while (Application::getInstance()->update())
	{
		cameraControl->update();
		//绘制
		render();


	}

	//清理操作
	delete nanali_texture;
	delete xiaoju_texture;

	Application::getInstance()->destroy();

	return 0;
}