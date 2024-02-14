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

static const float weights[5] = { 0.0545, 0.2442, 0.4026, 0.2442, 0.0545 };

float2 Distortion(VSOut In)
{
    float2 UV = In.Pos.xy / float2(resolution.x, resolution.y);
    float2 frequency =
    float2
    (
        noise.Sample(anisotropicSampler, In.UV + elapsedTimeCB * 0.2f).x
        , noise.Sample(anisotropicSampler, In.UV + float2(0.1f, 0.0f) + elapsedTimeCB * 0.2f).x
    );
    
    frequency -= frequency / 2.f;
    UV += frequency * 0.05f;
    
    return UV;
}

float4 main(VSOut In) : SV_Target
{
    float3 renderTargetColor = renderTarget.Sample(postProcessingSampler, In.UV).rgb;
    float3 postProcessColor = postProcessing.Sample(postProcessingSampler, In.UV).rgb;
    //strength = 1.0f;
    float3 color = renderTargetColor + (postProcessColor * /*strength*/2.0f);
    //color = postProcessColor;
    return float4(color, 1.0f);
}
