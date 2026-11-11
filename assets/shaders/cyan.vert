#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 uv;
out vec3 worldposition;

void main() {
  vec4 transformPosition = modelMatrix * vec4(aPos, 1.0);
  transformPosition = projectionMatrix * viewMatrix * transformPosition;
  gl_Position = transformPosition;
  worldposition = transformPosition.xyz;
  uv = aUV;
}