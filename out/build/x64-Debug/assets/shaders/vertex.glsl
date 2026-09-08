#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColors;
layout(location = 2) in vec2 aUV;

uniform float time;

uniform float speed;

out vec3 color;
out vec2 uv;

void main()
	{
		//x坐标的偏移量
		float dx = sin(time * speed)*0.3;
		gl_Position = vec4(aPos.x + dx , aPos.y , aPos.z , 1.0);
		//gl_Position = vec4(aPos.x , aPos.y , aPos.z , 1.0);

		color = aColors*(cos(time)+1.0)/2.0;
		uv = aUV;
	}