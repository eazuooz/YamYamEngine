#include "globals.hlsli"


struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};


VSOut main(VSIn In)
{
    VSOut output = (VSOut) 0.f;
    
    float4 pos = mul(float4(In.Pos, 1.0f), world);
    float4 viewPos = mul(float4(pos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    //x y z w
    
    output.Pos = projPos;
    output.WorldPos = pos;
    output.Color = In.Color;
    output.UV = In.UV;
    
    return output;
}
