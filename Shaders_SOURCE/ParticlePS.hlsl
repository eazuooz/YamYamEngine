#include "globals.hlsli"


struct GSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    uint Instance : SV_InstanceID;
};

float4 main(GSOut In) : SV_Target
{
    float4 Color = (float4) 0.f;
        
    Color = albedo.Sample(anisotropicSampler, In.UV);
    
    if (Color.a <= 0.0f)
        discard;
    
    Color.rgb *= startColor.rgb;
    
    return Color;

}