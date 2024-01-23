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
    //float4 Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    //return Color;
    
    //float2 UV = Distortion(In);
    //Color = postProcessing.Sample(postProcessingSampler, UV);
    
    float4 Color = postProcessing.Sample(postProcessingSampler, In.UV);
    return Color;
    
    
    //int i;
    //for (i = 0; i < 5; i++)
    //{
    //    Color.rgb += weights[i] *
    //    postProcessing.Sample(postProcessingSampler, In.UV 
    //        + float2(1.0f / 1600.0f / 32, 0.0f) * float(i - 2)).rgb;
    //}
    
    //for (i = 0; i < 5; i++)
    //{
    //    Color.rgb += weights[i] *
    //    postProcessing.Sample(postProcessingSampler, In.UV 
    //        + float2(0.0f, 1.0f / 900.0f / 32) * float(i - 2)).rgb;
    //}
   
    //////Color = postProcessing.Sample(postProcessingSampler, In.UV);
    //float l = (Color.r + Color.g + Color.b) / 3;
    
    //const float threshold = 0.8f;
    
    //if (l < threshold)
    //    Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    

    //float4 origin = postProcessing.Sample(anisotropicSampler, In.UV);
    
    //return float4(origin.rgb + 1.0f * Color.rgb, 1.0f);
}
