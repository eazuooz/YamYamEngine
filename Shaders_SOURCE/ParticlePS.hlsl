#include "globals.hlsli"


struct VSIn
{
    float3 Pos : POSITION;
    uint iInstance : SV_InstanceID;
};

struct VSOut
{
    float4 Position : SV_Position;
};



float4 main(VSOut _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vColor;

}