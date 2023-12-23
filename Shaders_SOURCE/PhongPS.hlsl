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
    //////==================================
    
    float4 Output = albedoTexture.Sample(anisotropicSampler, input.UV);
    Output.rgb *= color;
    
    
    return Output;
}