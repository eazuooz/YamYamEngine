
struct VSIn
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VSOut
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


cbuffer Transform : register(b0)
{
    //float4 cbPos;
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
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