#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<assert.h>
#include"wrapper/checkError.h"
#include"application/application.h"
#include"glframework/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include"application/stb_image.h"

GLuint vao;
GLuint texture;
Shader* shader = nullptr;

//视口大小回调函数
void onResize(int width, int height)
{
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "on" << std::endl;
}

//键盘回调函数
void keyCallback(int key, int action, int mods)
{
	std::cout << key << std::endl;
}

void prepareShader()
{
	shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

//绑定创建的vbo
void prepareSingleBufferVBO()
{
	//顶点数据
	float posiVBO[36] = {
		-0.5f ,-0.5f, 0.0f,
		0.5f ,-0.5f, 0.0f,
		0.0f ,0.5f, 0.0f,
	};
	//uv数据
	float uvs[6] = {
		0.0f,0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};

	//颜色数据
	float colorVBO[18] = {
		1.0f , 0.0f , 0.0f,
		0.0f, 1.0f , 0.0f,
		0.0f , 0.0f, 1.0f,
	};

	//ebo数据
	unsigned int indices[3] = {
		0 , 1 , 2
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
	//shader->setInt("sampler", 0);

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
	//---------stbImage 读取图片-------------//
 
	//图片 宽度 ， 高度 ， rgba还是rab格式
	int width, height, channels;

	//反转y轴
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("assets/texture/xiaojuzhihua.png", &width, &height, &channels, STBI_rgb_alpha);


	//生成纹理并激活单元绑定
	glGenTextures(1, &texture);

	//激活纹理单元
	glActiveTexture(GL_TEXTURE0);

	//绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, texture);

	//传输纹理数据   同时开辟显存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//释放数据
	stbi_image_free(data);

	//设置纹理过滤
	//当纹理像素过低时用插值算法过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//更高的时候则用精准过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

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

	//设置OpenGL视口
	GL_CALL(glViewport(0, 0, 800, 600));

	//画布清理颜色
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 0.1f));

	//创建vao
	prepareSingleBufferVBO();

	//创建Shader
	prepareShader();

	//读取纹理
	//prepareTexture();

	//执行窗体循环
	while (Application::getInstance()->update())
	{
		//绘制
		render();
	}

	//清理操作
	Application::getInstance()->destroy();

	return 0;
}