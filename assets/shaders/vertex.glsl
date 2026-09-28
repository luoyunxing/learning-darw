#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColors;
layout(location = 2) in vec2 aUV;

uniform float time;
uniform float speed;
uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;
out vec2 uv;

void main()
	{
		//x坐标的偏移量
		//float dx = sin(time * speed)*0.3;
		//gl_Position = vec4(aPos.x + dx , aPos.y , aPos.z , 1.0);


		//随时间变小
		//float scale = 1.0 / time;
		//vec3 sPos = aPos * scale;
		//gl_Position = vec4(sPos, 1.0);


		//让纹理旋转
		//vec4 position = vec4(aPos,1.0);
		//position = transform*position;
		//gl_Position = position;

		//普通输出
		//gl_Position = vec4(aPos.x , aPos.y , aPos.z , 1.0);

		//摄像机输出
		vec4 position = vec4(aPos, 1.0);
		position = projectionMatrix*viewMatrix*position;
		gl_Position = position;

		
		//将数据传入fragmentSharer
		color = aColors*(cos(time)+1.0)/2.0;
		uv = aUV;
	}