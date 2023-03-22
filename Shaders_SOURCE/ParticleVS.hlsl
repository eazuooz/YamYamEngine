#include "globals.hlsli"

struct VSIn
{
    float4 LocalPosition : POSITION;
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
  
    Out.LocalPosition = In.LocalPosition;
    Out.Instance = In.Instance;
        
    return Out;
}

