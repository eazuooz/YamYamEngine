#include "globals.hlsli"


struct GSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(GSOut In) : SV_Target
{
    float4 Color = (float4) 0.f;
        
    Color = triangleTexture.Sample(anisotropicSampler, In.UV);
    
    return Color;

}