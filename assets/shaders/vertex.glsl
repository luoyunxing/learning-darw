#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColors;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aNormal;

uniform float time;
uniform float speed;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;
out vec2 uv;
out vec3 normal;
out vec3 worldposition;

void main() {

  // 摄像机输出
  vec4 transformPosition = modelMatrix * vec4(aPos, 1.0);
  transformPosition = projectionMatrix * viewMatrix * transformPosition;
  gl_Position = transformPosition;

  // 将数据传入fragmentSharer
  worldposition = transformPosition.xyz;
  color = aColors * (cos(time) + 1.0) / 2.0;
  uv = aUV;
  normal = aNormal;
}