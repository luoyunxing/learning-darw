#pragma once
#include "object.h"

class Scene : public Object
{
public:
	Scene() 
	{
		mType = ObjectType::Scene;
	}
	~Scene() {}
};