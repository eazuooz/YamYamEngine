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

VSOut main(VSIn In)
{
    //VSOut output;
    
    //output.Pos = float4(In.Pos, 1.0);
    //output.UV = In.UV;

    //return output;
    
    VSOut output = (VSOut) 0.f;
    
    float4 pos = mul(float4(In.Pos, 1.0f), world);
    float4 viewPos = mul(float4(pos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    output.Pos = projPos;
    output.UV = In.UV;
    
    return output;
}