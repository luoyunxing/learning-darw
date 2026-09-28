#include"texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../application/stb_image.h"

Texture::Texture(const std::string& path, unsigned int textureUnit)
{
	//---------stbImage 读取图片-------------//

	//图片 宽度 ， 高度 ， rgba还是rab格式
	int channels;
	m_TextureUnit = textureUnit;

	//反转y轴
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);


	//生成纹理并激活单元绑定
	glGenTextures(1, &m_Texture);

	//激活纹理单元
	glActiveTexture(GL_TEXTURE0 + m_TextureUnit);

	//绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	//传输纹理数据   同时开辟显存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//使用mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	//释放数据
	stbi_image_free(data);

	//设置纹理过滤
	//当纹理像素过低时用插值算法过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//更高的时候则用精准过滤
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//增加mipmap过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::~Texture()
{
	if (m_Texture != 0)
	{
		glDeleteTextures(1, &m_Texture);
	}
}
