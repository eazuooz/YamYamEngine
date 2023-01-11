// HLSL 로 VertexShader 작성하기
#include "globals.hlsli"


float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 color = (float) 0.0f;
    
    color = triangleTexture.Sample(g_sam_0, _in.vUV);
    
    //if (int_0 == 1)
    //{
    //    color = (float)0.0f;

    //}
    
    return color;
}