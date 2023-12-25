#include "globals.hlsli"

struct VS_In
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_Out
{
    float4 ProjPosition : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
    float3 Color : COLOR; // Normal lines 쉐이더에서 사용
};

float4 main(VS_Out input) : SV_TARGET
{
    return specularCube.Sample(anisotropicSampler, input.WorldPosition.xyz);
    
    //float4 color = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    //return color;
}