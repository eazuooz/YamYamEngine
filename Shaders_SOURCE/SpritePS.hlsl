// HLSL 로 VertexShader 작성하기
#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut Out) : SV_Target
{
    float4 color = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    if (animationType == 1) // 2D
    {
        float2 diff = (atlasSize - spriteSize) / 2.0f;
        float2 UV = (leftTop - diff - offset) + (atlasSize * Out.UV);
        
        if (UV.x < leftTop.x || leftTop.x + spriteSize.x < UV.x
            || UV.y < leftTop.y || leftTop.y + spriteSize.y < UV.y)
        {
            discard;
        }
        color = atlasTexture.Sample(anisotropicSampler, UV);
    }
    else if (animationType == 2) // 3D
    {
        
    }
    else
    {
        color = triangleTexture.Sample(anisotropicSampler, Out.UV);
    }
   
    //Light
    LightColor lightColor = (LightColor) 0.0f;
    
    for (int i = 0; i < numberOfLight; i++)
    {
        CalculateLight2D(lightColor, Out.WorldPos.xyz, i);
    }
    
    color *=  lightColor.diffuse;
    
    return color;
}