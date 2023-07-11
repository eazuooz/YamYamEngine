#include "globals.hlsli"


struct VSIn
{
    float4 Pos : POSITION;
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
        
    // VS_OUT ���� ������ SV_Position ���� PixelShader �� �Էµ� �� �ȼ���ǥ�� ��ȯ�ؼ� �Է�
    float2 UV = In.Pos.xy / resolution;
        
    float2 frequency = 
    float2
    (
        noise.Sample(anisotropicSampler, In.UV + elapsedTime * 0.2f).x
        , noise.Sample(anisotropicSampler, In.UV + float2(0.1f, 0.0f) + elapsedTime * 0.2f).x
    );
    
    frequency -= frequency / 2.f;
    UV += frequency * 0.05f;
    
    Color = postProcessing.Sample(anisotropicSampler, UV);
    
    return Color;
}

