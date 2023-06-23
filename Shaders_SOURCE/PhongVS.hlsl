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

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT Output;
    return Output;
}