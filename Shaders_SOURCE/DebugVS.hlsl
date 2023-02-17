#include "globals.hlsli"


float4 main( float4 pos : POSITION ) : SV_POSITION
{
    float4 localPos = mul(pos, world);
    float4 viewPos = mul(localPos, view);
    float4 projPos = mul(viewPos, projection);
    
    return projPos;
}