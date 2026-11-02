#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../wrapper/checkError.h"
#include<string>
#include<fstream>
#include<sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Geometry {
public:
	Geometry();
	~Geometry();

	static Geometry* createBox(float size);
	static Geometry* createSphere(float radius);
	static Geometry* createFlat(float width, float height);

	GLuint getVao()const 
	{ 
		return mVao; 
	}

	uint32_t getIndicesCount() const 
	{
		return mIndicesCount;
	}

private:
	GLuint mVao{ 0 };
	GLuint mPosVbo{ 0 };
	GLuint mUvVbo{ 0 };
	GLuint mNormalVbo{ 0 };
	GLuint mEbo{ 0 };

	uint32_t mIndicesCount{ 0 };
};