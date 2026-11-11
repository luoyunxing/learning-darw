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

// --- 统一光源参数 ---
uniform int LightType; // 0 = 平行光, 1 = 点光源, 2 = 探照灯

uniform vec3 LightPosition;  // 点光源/探照灯的位置 (平行光不用)
uniform vec3 LightDirection; // 平行光的方向 / 探照灯的朝向
uniform vec3 LightColor;     // 光源颜色

// 光锥参数 (探照灯专用)
uniform float CutOff;
uniform float OuterCutOff;

// 距离衰减参数 (点光源/探照灯专用)
uniform float Constant;
uniform float Linear;
uniform float Quadratic;

// 环境光
uniform vec3 AmbientColor;

// 光斑大小、强度 (材质参数)
uniform float SpotSize;     // 高光收敛度
uniform float Instensity;   // 高光强度
uniform float LightEdge;    // 边缘光照
uniform float FresnelPower; // 菲涅尔系数

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

  // 环境光 (所有灯光类型共用)
  vec3 ambientColor = ObjectColor * AmbientColor;

  // 基础向量
  vec3 normalN = normalize(normal);
  vec3 viewDir = normalize(CameraPosition - worldposition);

  // ==========================================
  // 1. 计算光线方向 (lightDirN 永远从物体指向光源)
  // ==========================================
  vec3 lightDirN;
  if (LightType == 0) {
    // 平行光：方向固定 (你原来的逻辑是 -LightDirection)
    lightDirN = normalize(-LightDirection);
  } else {
    // 点光源/探照灯：动态计算位置方向
    lightDirN = normalize(LightPosition - worldposition);
  }

  // ==========================================
  // 2. 计算光锥强度 (仅探照灯需要)
  // ==========================================
  float spotIntensity = 1.0; // 默认全亮
  if (LightType == 2) {
    float theta = dot(normalize(-lightDirN), normalize(LightDirection));
    float epsilon = CutOff - OuterCutOff;
    spotIntensity = clamp((theta - OuterCutOff) / epsilon, 0.0, 1.0);

    // 如果完全在光锥外，直接输出环境光并退出
    if (spotIntensity <= 0.0) {
      FragColor = vec4(ambientColor, 1.0);
      return;
    }
  }

  // ==========================================
  // 3. 计算距离衰减 (点光源和探照灯需要)
  // ==========================================
  float attenuation = 1.0; // 默认无衰减
  if (LightType == 1 || LightType == 2) {
    float distance = length(LightPosition - worldposition);
    attenuation = 1.0 / (Constant + Linear * distance +
                         Quadratic * (distance * distance));
  }

  // ==========================================
  // 4. 计算漫反射 (所有灯光共用)
  // ==========================================
  float diffuse = clamp(dot(lightDirN, normalN), 0.0, 1.0);
  vec3 diffuseColor = ObjectColor * LightColor * diffuse;

  // ==========================================
  // 5. 计算高光反射 (所有灯光共用)
  // ==========================================
  vec3 halfDir = normalize(lightDirN + viewDir);
  float NdotH = clamp(dot(normalN, halfDir), 0.0, 1.0);
  float specular = pow(NdotH, SpotSize) * Instensity;

  // 菲涅尔
  float fresnel =
      pow(1.0 - clamp(dot(normalN, viewDir), 0.0, 1.0), FresnelPower);
  specular *= mix(1.0, LightEdge, fresnel);
  vec3 specularColor = LightColor * specular;

  // ==========================================
  // 6. 最终合成
  // ==========================================
  // 乘上衰减和光锥强度（对于平行光，这俩都是 1.0，不影响）
  diffuseColor *= attenuation * spotIntensity;
  specularColor *= attenuation * spotIntensity;

  vec3 finalColor = specularColor + diffuseColor + ambientColor;

  FragColor = vec4(finalColor.rgb, 1.0);
}