#include "globals.hlsli"

struct VS_INPUT
{
    float3 mPosition : POSITION;
    float2 mUV : TEXCOORD;
    float3 mNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 mPosition : SV_Position;
    float2 mUV : TEXCOORD;
    float3 mNormal : NORMAL;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 Output = float4(1.f, 0.f, 1.f, 1.f);
    return Output;
}