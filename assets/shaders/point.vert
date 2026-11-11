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
out vec3 worldposition; // 真正的世界坐标

void main() {
  // 1. 先算出世界坐标（用于后续光照计算和相机视线计算）
  vec4 worldPos = modelMatrix * vec4(aPos, 1.0);
  worldposition = worldPos.xyz;

  // 2. 再算出裁剪空间坐标（用于最终渲染输出）
  gl_Position = projectionMatrix * viewMatrix * worldPos;

  // 3. 将数据传入 fragmentShader
  uv = aUV;
  normal = normalMatrix * aNormal;
}