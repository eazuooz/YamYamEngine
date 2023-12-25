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


float4 main(VS_OUT input) : SV_Target
{
    float3 toEye = normalize(cameraPosition.xyz - input.WorldPosition.xyz);
    float3 viewReflect = reflect(-toEye, input.WorldNormal);
    
    float4 diffuse = diffuseCube.Sample(anisotropicSampler, input.WorldNormal);
    float4 specular = specularCube.Sample(anisotropicSampler, viewReflect);
    
    diffuse *= float4(mat.diffuseColor.rgb, 1.0);
    specular *= pow((specular.r + specular.g + specular.b) / 3.0, mat.shininess);
    specular *= float4(mat.specularColor.rgb, 1.0);
    
    float4 Output = diffuse + specular; //specularCube.Sample(anisotropicSampler, viewReflect);

    return Output;
}