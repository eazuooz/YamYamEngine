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

float4 main(VSOut In) : SV_Target
{
    float4 Color = (float4) 0.0f;
        
    float2 UV = In.Pos.xy / float2(1600.0f, 900.0f); 
    float2 frequency = 
    float2
    (
        noise.Sample(anisotropicSampler, In.UV + elapsedTime * 0.2f).x
        , noise.Sample(anisotropicSampler, In.UV + float2(0.1f, 0.0f) + elapsedTime * 0.2f).x
    );
    
    frequency -= frequency / 2.f;
    UV += frequency * 0.05f;
    
    //msaa
    //int pixelX = 1600 * UV.x;
    //int pixelY = 900 * UV.y;
    //float4 samples = postProcessing.Load(int2(pixelX, pixelY), 0);
    
    float4 samples = postProcessing.Sample(anisotropicSampler, UV);
    
    
    return samples;
}
