#include "globals.hlsli"

static const float weights[5] = { 0.0545, 0.2442, 0.4026, 0.2442, 0.0545 };

//cbuffer SamplingPixelConstantData : register(b8)
//{
//    float dx;
//    float dy;
//    float threshold;
//    float strength;
//    float4 options;
//};

struct SamplingPixelShaderInput
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(SamplingPixelShaderInput input) : SV_TARGET
{
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        color += weights[i] * postProcessing.Sample
        (
            postProcessingSampler, 
            input.UV + float2(0.0f, dy) * float(i - 2)
        ).rgb;
    }
    
    return float4(color, 1.0f);
}