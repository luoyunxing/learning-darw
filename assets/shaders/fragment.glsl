#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 uv;
in vec3 normal;

uniform sampler2D nanali_sampler;
uniform sampler2D xiaoju_sampler;
uniform sampler2D earth_sampler;
uniform float time;
uniform vec3 uColor;

void main() {
  // float intensity = (sin(time)+1.0)/2.0;

  // FragColor = vec4(vec3(intensity)+color, 1.0f);

  // FragColor = texture(nanali_sampler , uv);

  vec3 normalN = normalize(normal);
  vec3 normalColor = clamp(normal, 0.0, 1.0);

  vec4 nanali_Color = texture(nanali_sampler, uv);
  vec4 xiaoju_Color = texture(xiaoju_sampler, uv);
  vec4 earth_Color = texture(earth_sampler, uv);
  // 纹理混合
  vec4 finalColor = nanali_Color * 0.2 + xiaoju_Color * 0.8;

  // FragColor = vec4(xiaoju_Color.rgb, 1.0);

  FragColor = vec4(normalColor.rgb, 1.0);
}