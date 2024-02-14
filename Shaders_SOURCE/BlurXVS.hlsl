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
    VSOut output;
    
    output.Pos = float4(In.Pos, 1.0);
    output.UV = In.UV;

    return output;
}