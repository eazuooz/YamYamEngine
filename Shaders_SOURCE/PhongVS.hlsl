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

VS_OUT main(VS_IN input)
{
    VS_OUT Output;
    
    
    VS_OUT output = (VS_OUT) 0.f;
    
    float4 pos = mul(float4(In.Pos, 1.0f), world);
    float4 viewPos = mul(float4(pos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    output.Position = projPos;
    output.Normal = input.Normal;
    output.UV = input.UV;
    
    return Output;
}