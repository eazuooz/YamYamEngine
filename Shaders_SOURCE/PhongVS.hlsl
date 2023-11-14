#include "globals.hlsli"

struct VS_IN
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    float3 Tangent : TANGENT;
    
    float4 BlendWeights : BLENDWEIGHT;
    float4 BlendIndices : BLENDINDICES;

};

struct VS_OUT
{
    float4 ProjPosition : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
    float Depth : FOG;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0.0f;
    
    if (animationType == 2)
    {
        Skinning(input.Position, input.Tangent, input.BiNormal, input.Normal
        , input.BlendWeights, input.BlendIndices, 0);
    }
    
    float4 worldPos = mul(float4(input.Position, 1.0f), world);
    float4 viewPos = mul(float4(worldPos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    float3 worldNormal = mul(float4(input.Normal, 1.0f), world).xyz;
    worldNormal = normalize(worldNormal);
    
    output.WorldPosition = worldPos;
    output.ProjPosition = projPos;
    output.WorldNormal = worldNormal;
    output.Depth = projPos.z / projPos.w;
    
    //output.LocalNormal = input.Normal;
    
    output.UV = input.UV;
    
    return output;
}