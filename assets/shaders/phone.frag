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

// ==========================================
// 光源数组定义 (使用结构体，适配 C++ 的传参)
// ==========================================
#define MAX_LIGHTS 16

struct Light {
  int type; // 0: 平行光, 1: 点光源, 2: 探照灯
  vec3 position;
  vec3 direction;
  vec3 color;
  float constant;
  float linear;
  float quadratic;
  float cutOff;
  float outerCutOff;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights; // 当前场景中实际的光源数量

// 光斑大小、强度 (材质参数)
uniform vec3 AmbientColor;
uniform float SpotSize;
uniform float Instensity;
uniform float LightEdge;
uniform float FresnelPower;

uniform vec3 CameraPosition;

void main() {
  // 贴图纹理
  vec4 nanali_Color = texture(nanali_sampler, uv);
  vec4 xiaoju_Color = texture(xiaoju_sampler, uv);
  vec4 earth_Color = texture(earth_sampler, uv);
  vec4 sampler_Color = texture(sampler, uv);
  vec4 mix_Color = nanali_Color * 0.2 + xiaoju_Color * 0.8;

  // 物体颜色
  vec3 ObjectColor = sampler_Color.xyz;

  // 环境光 (基础底色)
  vec3 ambientColor = ObjectColor * AmbientColor;

  // 基础向量
  vec3 normalN = normalize(normal);
  vec3 viewDir = normalize(CameraPosition - worldposition);

  // 初始化最终累加光照 (先全黑)
  vec3 finalLighting = vec3(0.0);

  // ==========================================
  // 遍历计算每个光源
  // ==========================================
  for (int i = 0; i < numLights; i++) {

    // 1. 计算光线方向 (lightDirN 永远从物体指向光源)
    vec3 lightDirN;
    if (lights[i].type == 0) {
      // 平行光：方向固定
      lightDirN = normalize(-lights[i].direction);
    } else {
      // 点光源/探照灯：动态计算位置方向
      lightDirN = normalize(lights[i].position - worldposition);
    }

    // 2. 计算光锥强度 (仅探照灯需要)
    float spotIntensity = 1.0; // 默认全亮
    if (lights[i].type == 2) {
      float theta = dot(normalize(-lightDirN), normalize(lights[i].direction));
      float epsilon = lights[i].cutOff - lights[i].outerCutOff;
      spotIntensity =
          clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);

      // 如果完全在光锥外，跳过当前光源的计算
      if (spotIntensity <= 0.0) {
        continue;
      }
    }

    // 3. 计算距离衰减 (点光源和探照灯需要)
    float attenuation = 1.0; // 默认无衰减
    if (lights[i].type == 1 || lights[i].type == 2) {
      float distance = length(lights[i].position - worldposition);
      attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance +
                           lights[i].quadratic * (distance * distance));
    }

    // 4. 计算漫反射
    float diffuse = clamp(dot(lightDirN, normalN), 0.0, 1.0);
    vec3 diffuseColor = ObjectColor * lights[i].color * diffuse;

    // 5. 计算高光反射
    vec3 halfDir = normalize(lightDirN + viewDir);
    float NdotH = clamp(dot(normalN, halfDir), 0.0, 1.0);
    float specular = pow(NdotH, SpotSize) * Instensity;

    // 菲涅尔
    float fresnel =
        pow(1.0 - clamp(dot(normalN, viewDir), 0.0, 1.0), FresnelPower);
    specular *= mix(1.0, LightEdge, fresnel);
    vec3 specularColor = lights[i].color * specular;

    // 6. 累加当前光源贡献 (乘上衰减和光锥强度)
    finalLighting +=
        (diffuseColor + specularColor) * attenuation * spotIntensity;
  }

  // 最终合成：环境光 + 所有光源的累加光照
  vec3 finalColor = ambientColor + finalLighting;

  FragColor = vec4(finalColor.rgb, 1.0);
}