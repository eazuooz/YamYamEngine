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
        
    Color = triangleTexture.Sample(anisotropicSampler, In.UV);
    
    //float fRatio = ParticleBufferGS[In.Instance] / ParticleBuffer[_in.iInstance].fMaxTime;
    //vColor.rgb *= lerp(StartColor, EndColor, fRatio).rgb;
    
    return Color;

}