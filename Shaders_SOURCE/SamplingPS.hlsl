#include "globals.hlsli"


struct VSIn
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_TARGET
{
    float3 color = postProcessing.Sample(postProcessingSampler, In.UV);
    float l = (color.r + color.g + color.b) / 3;
    
    //threshold = 0.5f;
    
    return l > threshold /*0.037f*/ ? float4(color, 1.0f) : float4(0.0f, 0.0f, 0.0f, 1.0f);
}