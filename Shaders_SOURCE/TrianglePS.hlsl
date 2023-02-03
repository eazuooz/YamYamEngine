// HLSL 로 VertexShader 작성하기
#include "globals.hlsli"


float4 main(VSOut _in) : SV_Target
{
    float4 color = (float) 0.0f;
    color = triangleTexture.Sample(g_sam_0, _in.vUV);
    
    return color;
}