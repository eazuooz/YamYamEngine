#include "globals.hlsli"

struct VSIn
{
    float3 LocalPosition : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 LocalPosition : SV_Position;
    uint Instance : SV_InstanceID;
};

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
  
    Out.LocalPosition = float4(In.LocalPosition, 1.0f);
    Out.Instance = In.Instance;
        
    return Out;
}

