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
        
    //// VS_OUT ���� ������ SV_Position ���� PixelShader �� �Էµ� �� �ȼ���ǥ�� ��ȯ�ؼ� �Է�
    float2 UV = In.Pos.xy / float2(1600.0f, 900.0f); //csbuffer grid(resolution ��밡��)
        
    float2 frequency = 
    float2
    (
        noise.Sample(anisotropicSampler, In.UV + elapsedTime * 0.2f).x
        , noise.Sample(anisotropicSampler, In.UV + float2(0.1f, 0.0f) + elapsedTime * 0.2f).x
    );
    
    frequency -= frequency / 2.f;
    UV += frequency * 0.05f;
    
    Color = postProcessing.Sample(anisotropicSampler, UV);
    
    //int pixelX = 1600 * UV.x;
    //int pixelY = 1600 * UV.y;
    
    //float4 samples = postProcessing.Load(int2(pixelX, pixelY), 0);
    //samples = postProcessing.Load(int2(pixelX, pixelY), 1);
    //samples = postProcessing.Load(int2(pixelX, pixelY), 2);
    //samples = postProcessing.Load(int2(pixelX, pixelY), 3);
    //Color.r = 1.0f;
    
    return Color;
}
