#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColors;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec2 uv;
out vec3 normal;
out vec3 worldposition;

void main() {
  vec4 transformPosition = modelMatrix * vec4(aPos, 1.0);
  transformPosition = projectionMatrix * viewMatrix * transformPosition;
  gl_Position = transformPosition;

  // 将数据传入fragmentSharer
  worldposition = transformPosition.xyz;
  uv = aUV;
  normal = normalMatrix * aNormal;
}