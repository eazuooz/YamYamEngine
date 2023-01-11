#include "globals.hlsli"





VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    float4 pos = mul(float4(_in.vPos, 1.0f), world);
    float4 viewPos = mul(float4(pos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    output.vPos = projPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}
