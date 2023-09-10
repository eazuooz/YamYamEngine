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
                   float3 toEye, Material mat, float4 color)
{
    // Blin phong
    float3 halfway = normalize(toEye + lightVec);
    float hdotn = dot(halfway, normal);
    float3 specular = mat.specular.rgb * pow(max(hdotn, 0.0f), mat.shininess * 2.0);
    
    // Phong
    //float3 r = -reflect(lightVec, normal);
    //float3 specular = mat.specular * pow(max(dot(toEye, r), 0.0f), mat.shininess);
    //return mat.ambient + (mat.diffuse + specular) * lightStrength;
    
    return mat.ambient.rgb + (mat.diffuse.rgb + specular) * lightStrength * color;
}

float3 ComputeDirectionalLight(LightAttribute L, Material mat, float3 normal,
                                float3 toEye)
{
    float3 lightVec = L.direction.xyz;
    
    normal = float3(0.0f, 0.0f, 1.0f);
    
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.power * ndotl;
    
    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat, L.color);
}

float CalcAttenuation(float d, float radius)
{
    return saturate((radius - d) / radius);
}

float3 ComputePointLight(LightAttribute L, Material mat, float3 pos, float3 normal,
                          float3 toEye)
{
    float3 lightVec = L.position.xyz - pos;

    // 쉐이딩할 지점부터 조명까지의 거리 계산
    float d = length(lightVec);

    // 너무 멀면 조명이 적용되지 않음
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

        return BlinnPhong(lightStrength, lightVec, normal, toEye, mat, L.color);
    }
}

float3 ComputeSpotLight(LightAttribute L, Material mat, float3 pos, float3 normal,
                         float3 toEye)
{
    float3 lightVec = L.position.xyz - pos;

    // 쉐이딩할 지점부터 조명까지의 거리 계산
    float d = length(pos - L.position.xyz);

    // 너무 멀면 조명이 적용되지 않음
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

        float spotFactor = pow(max(-dot(lightVec, L.direction), 0.0f), L.power);
        lightStrength *= spotFactor;

        return BlinnPhong(lightStrength, lightVec, normal, toEye, mat, L.color);
    }
    
    // if에 else가 없을 경우 경고 발생
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
            color += ComputeDirectionalLight(lights[i], mat, input.WorldNormal, toEye);
        else if (lights[i].type == POINT_LIGHT)
            color += ComputePointLight(lights[i], mat, input.WorldPosition.xyz, input.WorldNormal, toEye);
        else if (lights[i].type == SPOT_LIGHT)
            color += ComputeSpotLight(lights[i], mat, input.WorldPosition.xyz, input.WorldNormal, toEye);
    }
    
    //color += ComputePointLight(lights[1], mat, input.WorldPosition.xyz, input.WorldNormal, toEye);
        
    float4 Output = albedo.Sample(anisotropicSampler, input.UV);
    Output.rgb *= color;
    
    //Output.rgb = input.Depth;
    
    return Output;
}