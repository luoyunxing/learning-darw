#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 uv;
in vec3 normal;
in vec3 worldposition;

// 纹理
uniform sampler2D nanali_sampler;
uniform sampler2D xiaoju_sampler;
uniform sampler2D earth_sampler;

// 平行光
uniform vec3 LightDirection;
uniform vec3 LightColor;

//
uniform float time;
uniform vec3 uColor;
uniform vec3 CameraPosition;

void main() {
  // float intensity = (sin(time)+1.0)/2.0;
  // FragColor = vec4(vec3(intensity)+color, 1.0f);
  // FragColor = texture(nanali_sampler , uv);

  // 贴图纹理
  vec4 nanali_Color = texture(nanali_sampler, uv);
  vec4 xiaoju_Color = texture(xiaoju_sampler, uv);
  vec4 earth_Color = texture(earth_sampler, uv);
  vec4 finalColor = nanali_Color * 0.2 + xiaoju_Color * 0.8;

  // 物体颜色
  vec3 ObjectColor = nanali_Color.xyz;

  // 漫反射
  vec3 normalN = normalize(normal);
  vec3 lightDirN = normalize(LightDirection);
  float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);

  // 实现漫反射
  vec3 final_Color = ObjectColor * LightColor * diffuse;

  // FragColor = vec4(final_Color.rgb, 1.0);

  //
  vec3 viewDir = normalize(worldposition - CameraPosition);
  viewDir = clamp(-viewDir, 0.0, 1.0);
  // FragColor = vec4(final_Color, 1.0);
  FragColor = vec4(viewDir, 1.0);
}