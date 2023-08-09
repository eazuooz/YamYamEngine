#include "globals.hlsli"

struct VS_IN
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUT
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

float4 main(VS_OUT input) : SV_Target
{
    float4 Output = float4(1.f, 0.f, 1.f, 1.f);
    return Output;
}