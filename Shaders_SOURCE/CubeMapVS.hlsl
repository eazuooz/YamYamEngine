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


VS_Out main(VS_In input) 
{
    VS_Out output = (VS_Out) 0.0f;
    
    float4 pos = float4(input.Position, 0.0f);
    //pos = mul(pos, world); //identify matrix
    output.WorldPosition = float4(pos.xyz, 1.0f);
    
    //matrix invTranspose = world;
    //invTranspose._41_42_43 = float3(0.0f);
    //invTranspose = 
    
    float4 normal = float4(input.Normal, 0.0f);
    //output.WorldNormal = mul(normal, world).xyz;
    output.WorldNormal = normalize(output.WorldNormal);

    //pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.ProjPosition = pos;
    output.UV = input.UV;
    output.Color = float3(1.0, 1.0, 0.0);
    
    return output;
}