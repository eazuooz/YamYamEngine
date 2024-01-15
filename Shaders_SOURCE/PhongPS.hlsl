#include "globals.hlsli"

struct VS_IN
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

struct VS_OUT
{
    float4 ProjPosition : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
    float Depth : FOG;
};

//#define DIRECTIONAL_LIGHT   0
//#define POINT_LIGHT         1
//#define SPOT_LIGHT          2
float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal,
                   float3 toEye, Material mat, float4 color, float2 uv)
{
    // Blin phong
    float3 halfway = normalize(toEye + lightVec);
    float hdotn = dot(halfway, normal);
    float3 specular = mat.specularColor.rgb * pow(max(hdotn, 0.0f), mat.shininess * 2.0);
    
    if (mat.usedSpecular)
    {
        specular = /*mat.specularColor.rgb*/ specularTexture.Sample(anisotropicSampler, uv).rgb;
    }
    //Phong

    //float3 r = -reflect(lightVec, normal);
    //float3 specular = mat.specular * pow(max(dot(toEye, r), 0.0f), mat.shininess);
    //return mat.ambient.rgb + (mat.diffuse.rgb + specular) * lightStrength;
    
    return /*mat.ambientColor.rgb*/float3(0.1f, 0.1f, 0.1f) + (mat.diffuseColor.rgb + specular) * lightStrength * color.rgb;
}

float3 ComputeDirectionalLight(LightAttribute L, Material mat, float3 normal,
                                float3 toEye, float2 uv)
{
    float3 lightVec = L.direction.xyz;
    
    float ndotl = max(dot(-lightVec, normal), 0.0f);
    float3 lightStrength = L.power * ndotl;
    
    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat, L.color, uv);
}

float CalcAttenuation(float d, float radius)
{
    return saturate((radius - d) / radius);
}

float3 ComputePointLight(LightAttribute L, Material mat, float3 pos, float3 normal,
                          float3 toEye, float2 uv)
{
    float3 lightVec = L.position.xyz - pos;

    // ���̵��� �������� ��������� �Ÿ� ���
    float d = length(lightVec);

    // �ʹ� �ָ� ������ ������� ����
    if (d > L.radius)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        lightVec /= d;
        
        float ndotl = max(dot(lightVec, normal), 0.0f);
        float3 lightStrength = L.power * ndotl;

        float att = CalcAttenuation(d, L.radius);
        lightStrength *= att;

        return BlinnPhong(lightStrength, lightVec, normal, toEye, mat, L.color, uv);
    }
}

float3 ComputeSpotLight(LightAttribute L, Material mat, float3 pos, float3 normal,
                         float3 toEye, float2 uv)
{
    float3 lightVec = L.position.xyz - pos;

    // ���̵��� �������� ��������� �Ÿ� ���
    float d = length(pos - L.position.xyz);

    // �ʹ� �ָ� ������ ������� ����
    if (d > L.radius)
    {
        return float3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        lightVec /= d;
        
        float ndotl = max(dot(lightVec, normal), 0.0f);
        float3 lightStrength = L.power * ndotl;

        float att = CalcAttenuation(d, L.radius);
        lightStrength *= att;

        float spotFactor = pow(max(-dot(lightVec, L.direction.xyz), 0.0f), L.power);
        lightStrength *= spotFactor;

        return BlinnPhong(lightStrength, lightVec, normal, toEye, mat, L.color, uv);
    }
    
    // if�� else�� ���� ��� ��� �߻�
    // warning X4000: use of potentially uninitialized variable
}

// Schlick approximation: Eq. 9.17 in "Real-Time Rendering 4th Ed."

float3 SchlickFresnel(float3 fresnelR0, float3 normal, float3 toEye)
{
    // ���� �ڷ��
    // THE SCHLICK FRESNEL APPROXIMATION by Zander Majercik, NVIDIA
    // http://psgraphics.blogspot.com/2020/03/fresnel-equations-schlick-approximation.html
    
    float normalDotView = saturate(dot(normal, toEye));

    float f0 = 1.0f - normalDotView; // 90���̸� f0 = 1, 0���̸� f0 = 0

    // 1.0 ���� ���� ���� ���� �� ���ϸ� �� ���� ���� �˴ϴ�.
    // 0�� -> f0 = 0 -> fresnelR0 ��ȯ
    // 90�� -> f0 = 1.0 -> float3(1.0) ��ȯ
    // 0���� ����� �����ڸ��� Specular ����, 90���� ����� ������ ���� ����(fresnelR0)
    return fresnelR0 + (1.0f - fresnelR0) * pow(f0, 5.0);
}

float4 main(VS_OUT input) : SV_Target
{
    float3 toEye = normalize(cameraPosition.xyz - input.WorldPosition.xyz);
    float3 color = (float3) 0.0f;
    
    [loop]
    for (uint i = 0; i < lightCount; ++i)
    {
        if (lights[i].type == DIRECTIONAL_LIGHT)
            color += ComputeDirectionalLight(lights[i], mat, input.WorldNormal, toEye, input.UV);
        else if (lights[i].type == POINT_LIGHT)
            color += ComputePointLight(lights[i], mat, input.WorldPosition.xyz, input.WorldNormal, toEye, input.UV);
        else if (lights[i].type == SPOT_LIGHT)
            color += ComputeSpotLight(lights[i], mat, input.WorldPosition.xyz, input.WorldNormal, toEye, input.UV);
    }
    
    
    //////==========rim light=================
    
    //const float rimPower = 1.5f;
    //const float3 rimColor = float3(1.0f, 0.0f, 1.0f);
    //const float rimStrength = 10.5f;
    
    //// Smoothstep
    //// https://thebookofshaders.com/glossary/?search=smoothstep       

    //float rim = (1.0f - dot(input.WorldNormal, toEye));
    ////float rim = (1.0f - dot(input.LocalNormal, toEye));
    ////�Ϲ������δ� worldNormal�� ��������� ���ӿ����� ȿ���� �ش�ȭ�ϱ����� LocalNormal�� ��õ
    //rim = smoothstep(0.0f, 1.0f, rim);
    
    //rim = pow(abs(rim), rimPower);
    //color += rim * rimColor * rimStrength;
    //////==========rim light=================
    
    float4 Output = albedoTexture.Sample(anisotropicSampler, input.UV);
    Output.rgb *= color;
    
    /// =============== IBL =============================
    float3 viewReflect = reflect(-toEye, input.WorldNormal);
    
    float4 diffuse = diffuseCube.Sample(anisotropicSampler, input.WorldNormal);
    float4 specular = specularCube.Sample(anisotropicSampler, viewReflect);
    
    diffuse *= float4(mat.diffuseColor.rgb, 1.0);
    specular *= pow((specular.r + specular.g + specular.b) / 3.0, mat.shininess);
    specular *= float4(mat.specularColor.rgb, 1.0);
    
    // ����: https://www.shadertoy.com/view/lscBW4
    // fresnelR0�� ������ ���� ����
    // Water : (0.02, 0.02, 0.02)
    // Glass : (0.08, 0.08, 0.08)
    // Plastic : (0.05, 0.05, 0.05)
    // Gold: (1.0, 0.71, 0.29)
    // Silver: (0.95, 0.93, 0.88)
    // Copper: (0.95, 0.64, 0.54)
    float3 f = SchlickFresnel(float3(1.0f, 0.71f, 0.29f), input.WorldNormal, toEye);
    specular.xyz *= f;
    
    diffuse *= albedoTexture.Sample(anisotropicSampler, input.UV);
    
    Output = diffuse + specular;
    ///=============== IBL =============================
    
    
    return Output;
}