#include "globals.hlsli"

float4 main(float4 position : SV_Position) : SV_TARGET
{
    float4 color = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return color;
}