// HLSL 로 VertexShader 작성하기

struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


cbuffer TRANSFORM : register(b0)
{
    float4 cbPos;
};

cbuffer Material : register(b1)
{
    int int_0;
    float float_0;
    float2 vec2_0;
    float3 vec3_0;
    float4 vec4_3;
    matrix mat_0;
}


Texture2D triangleTexture : register(t0);

SamplerState g_sam_0 : register(s0); // anisotropic sampler
SamplerState g_sam_1 : register(s1); // point sampler

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