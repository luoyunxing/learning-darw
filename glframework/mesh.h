#pragma once

#include "geometry.h"
#include "object.h"
#include "meterial/meterial.h"

class Mesh : public Object
{
public:
	Mesh(Geometry* geometry , Meterial* meterial);
	~Mesh();

public:
	//一个Mesh（模型）由几何体和材质构成 
	//在main.cpp 里先创建几何体和材质，再把他们传入mesh，
	//最后mesh再render队列里排队渲染
	Geometry* mGemetry{ nullptr };
	Meterial* mMeterial{ nullptr };
};