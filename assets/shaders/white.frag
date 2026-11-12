#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 worldposition;

//

uniform vec3 ObjectColor; // 新增颜色变量
void main() { FragColor = vec4(ObjectColor, 1.0); }