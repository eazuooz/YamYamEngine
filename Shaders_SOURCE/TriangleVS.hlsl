
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

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

SamplerState g_sam_0 : register(s0); // anisotropic sampler
SamplerState g_sam_1 : register(s1); // point sampler

VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = float4(_in.vPos + cbPos.xyz, 1.f);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    
    return output;
}
