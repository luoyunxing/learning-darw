#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
in vec3 worldposition;

// 纹理
uniform sampler2D sampler;
uniform sampler2D nanali_sampler;
uniform sampler2D xiaoju_sampler;
uniform sampler2D earth_sampler;

// 平行光
uniform vec3 LightDirection;
uniform vec3 LightColor;
// 环境光
uniform vec3 AmbientColor;
// 光斑大小、强度
uniform float SpotSize;
uniform float Instensity;
uniform float LightEdge;
uniform float FresnelPower;

uniform vec3 CameraPosition;

void main() {
  // float intensity = (sin(time)+1.0)/2.0;
  // FragColor = vec4(vec3(intensity)+color, 1.0f);
  // FragColor = texture(nanali_sampler , uv);

  // 贴图纹理
  vec4 nanali_Color = texture(nanali_sampler, uv);
  vec4 xiaoju_Color = texture(xiaoju_sampler, uv);
  vec4 earth_Color = texture(earth_sampler, uv);
  vec4 sampler = texture(sampler, uv);
  vec4 mix_Color = nanali_Color * 0.2 + xiaoju_Color * 0.8;

  // 物体颜色
  vec3 ObjectColor = earth_Color.xyz;

  // 环境光
  vec3 ambientColor = ObjectColor * AmbientColor;

  // 计算光照所
  vec3 normalN = normalize(normal);
  vec3 lightDirN = normalize(LightDirection);

  // 实现漫反射
  float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);
  vec3 diffuseColor = ObjectColor * LightColor * diffuse;

  // 实现高光反射
  vec3 viewDir = normalize(CameraPosition - worldposition);
  vec3 halfDir = normalize(-lightDirN + viewDir);
  float NdotH = clamp(dot(normalN, halfDir), 0.0, 1.0);
  float specular = pow(NdotH, SpotSize) * Instensity;
  // 菲涅尔：正对视角强，掠射角弱，让高光"收边"更自然
  float fresnel =
      pow(1.0 - clamp(dot(normalN, viewDir), 0.0, 1.0), FresnelPower);
  specular *= mix(1.0, LightEdge, fresnel);
  vec3 specularColor = LightColor * specular;

  vec3 finalColor = specularColor + diffuseColor + ambientColor;

  FragColor = vec4(finalColor.rgb, 1.0);
}