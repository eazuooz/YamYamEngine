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
    float4 origin2 = postProcessing.Sample(postProcessingSampler, In.UV);
    return origin2;
    
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}