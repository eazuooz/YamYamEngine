// HLSL 로 VertexShader 작성하기
#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut Out) : SV_Target
{
    float4 color = (float) 0.0f;
    color = albedo.Sample(anisotropicSampler, Out.UV);
    
    return color;
}