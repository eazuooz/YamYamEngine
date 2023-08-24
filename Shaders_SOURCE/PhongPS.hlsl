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
    
    return mat.ambient.rgb + (mat.diffuse.rgb + specular) * lightStrength;
}

float3 ComputeDirectionalLight(LightAttribute L, Material mat, float3 normal,
                                float3 toEye)
{
    float3 lightVec = L.direction.xyz;

    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.power * ndotl;
    //lightStrength = 1.0f;
    
    // Luna DX12 책에서는 Specular 계산에도
    // Lambert's law가 적용된 lightStrength를 사용합니다.
    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat, L.color);
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
        //else if (lights[i].type == POINT_LIGHT)
        //{
            
        //}
        //else if (lights[i].type == SPOT_LIGHT)
        //{
            
        //}
    }
        
    float4 Output = albedo.Sample(anisotropicSampler, input.UV);
    Output.rgb *= color;
    
    return Output;
}