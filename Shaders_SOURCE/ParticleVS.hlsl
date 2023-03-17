#include "globals.hlsli"



struct VSIn
{
    float4 Pos : POSITION;
    uint iInstance : SV_InstanceID;
};

struct VSOut
{
    float4 Position : SV_Position;
};

VSOut main(VSIn _in)
{
    VSOut Out = (VSOut) 0.f;
    
    //float3 vWorldPos = (_in.vPos * 50.f) + g_matWorld._41_42_43 + ParticleBuffer[ParticleIndex].vRelativePos.xyz;
    float3 WorldPos = (_in.Pos.xyz * 50.f) + world._41_42_43 + particleBuffer[_in.iInstance].position.xyz;
    
    float4 ViewPos = mul(float4(WorldPos, 1.f), view);
    Out.Position = mul(ViewPos, projection);
        
    return Out;
}

