#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>

//本texture类GPU资源封装类
//提供纹理数据的加载
//自动管理纹理对象生命周期
//纹理单元的手动绑定

class Texture
{
public:

	Texture(const std::string& path , unsigned int textureUnit);

	~Texture();

private:
	GLuint m_Texture{ 0 };
	int m_Width { 0 };
	int m_Height { 0 };
	int m_TextureUnit{ 0 };
};