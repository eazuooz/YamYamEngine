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

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0.0f;
    
    float4 worldPos = mul(float4(input.Position, 1.0f), world);
    float4 viewPos = mul(float4(worldPos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    float3 worldNormal = mul(float4(input.Normal, 1.0f), world).xyz;
    worldNormal = normalize(worldNormal);
    
    output.WorldPosition = worldPos;
    output.ProjPosition = projPos;
    output.WorldNormal = worldNormal;

    output.UV = input.UV;
    
    return output;
}