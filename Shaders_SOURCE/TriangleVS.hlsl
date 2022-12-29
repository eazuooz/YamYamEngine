
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
