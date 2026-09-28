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


GLuint vao;
Texture* nanali_texture;
Texture* xiaoju_texture;
Shader* shader = nullptr;

perspectivegraphicCamera* camera = nullptr;
//Trackballcameracontrol* cameraControl = nullptr;
GameCameraControl* cameraControl = nullptr;

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

//创建着色器
void prepareShader()
{
	shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

//绑定创建的vbo
void prepareSingleBufferVBO()
{
	//顶点数据
	/*float posiVBO[36] = {
		-0.5f ,-0.5f, 0.0f,
		0.5f ,-0.5f, 0.0f,
		0.0f ,0.5f, 0.0f,
		0.5f , 0.5f , 0.0f
	};*/

	//非NDC坐标顶点数据
	float posiVBO[36] = {
		-2.0f , 0.0f , 0.0f,
		2.0f , 0.0f , 0.0f,
		0.0f , 2.0f , 0.0f
	};

	//uv数据
	float uvs[8] = {
		0.0f,0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 1.0f
	};

	//颜色数据
	float colorVBO[18] = {
		1.0f , 0.0f , 0.0f,
		0.0f, 1.0f , 0.0f,
		0.0f , 0.0f, 1.0f,
	};

	//ebo数据
	unsigned int indices[6] = {
		0 , 1 , 2,
		2 , 1 , 3
	};

	//创建vbo,并绑定
	GLuint posivbo, colorvbo;
	GL_CALL(glGenBuffers(1, &posivbo));
	GL_CALL(glGenBuffers(1, &colorvbo));

	
	//绑定当前vbo，并写入位置数据
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posivbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(posiVBO),posiVBO, GL_STATIC_DRAW));

	//绑定当前vbo，并写入颜色数据
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorvbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colorVBO),colorVBO, GL_STATIC_DRAW));

	//创建uvvbo，并绑定
	GLuint uvvbo;
	glGenBuffers(1, &uvvbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//创建vao，并绑定
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//创建ebo，并绑定
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//将位置属性的描述信息加入到vao当中
	glBindBuffer(GL_ARRAY_BUFFER, posivbo);
	//绑定到0号属性
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

	//将颜色属性的描述信息加入到vao当中
	glBindBuffer(GL_ARRAY_BUFFER, colorvbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

	//将uv属性的描述信息加入到vao当中
	glBindBuffer(GL_ARRAY_BUFFER, uvvbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));

	//将ebo绑定到vao当中
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

	//解除vao绑定
	glBindVertexArray(0);
}

//渲染操作
void render()
{
	//画布清理
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//绑定到当前的program
	shader->begin();

	//更新uniform变量 time
	shader->setFloat("time", glfwGetTime());
	shader->setFloat("speed", 10.0);
	shader->setColor("uColor", 1, 1, 1);

	shader->setInt("nanali_sampler", 0);
	shader->setInt("xiaoju_sampler", 1);
	//shader->setMatrix44("transform", transform);
	shader->setMatrix44("viewMatrix", camera->getViewMatrix());
	shader->setMatrix44("projectionMatrix",camera->getprojectionMatrix());

	//绑定到当前的vao
	GL_CALL(glBindVertexArray(vao));

	//执行绘制
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT , 0);
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

}

//设置摄像机矩阵
void prepareCamera()
{
	float aspect = static_cast<float>(Application::getInstance()->getWidth()) / static_cast<float>(Application::getInstance()->getHeight());
	camera = new perspectivegraphicCamera(60.0f, aspect, 0.1f, 1000.0f);

	//决定初始位置
	camera->mPosition = glm::vec3(0.0f, 0.0f, 6.0f);

	//cameraControl = new Trackballcameracontrol();
	cameraControl = new GameCameraControl();
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

	//设置OpenGL视口
	GL_CALL(glViewport(0, 0, 800, 600));

	//画布清理颜色
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 0.1f));

	//创建vao
	prepareSingleBufferVBO();

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