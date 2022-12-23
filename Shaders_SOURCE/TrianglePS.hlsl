// HLSL 로 VertexShader 작성하기

struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


Texture2D triangleTexture : register(t0);
SamplerState samplerState : register(s0);

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 color = (float) 0.0f;
    
    color = triangleTexture.Sample(samplerState, _in.vUV);
    
    return color;
}