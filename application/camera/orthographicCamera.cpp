#include "orthographicCamera.h"


orthographicCamera::orthographicCamera(float l, float r, float t, float b, float n, float f)
{
	mLeft = l;
	mRight = r;
	mTop = t;
	mBottom = b;
	mNear = n;
	mFar = f;
}

orthographicCamera::~orthographicCamera()
{

}

glm::mat4 orthographicCamera::getprojectionMatrix()
{
	return glm::ortho( mLeft , mRight , mTop , mBottom , mNear , mFar );
}