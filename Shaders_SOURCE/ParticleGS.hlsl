#include "globals.hlsli"

struct VSOut
{
    float4 LocalPosition : SV_Position;
    uint Instance : SV_InstanceID;
};

struct GSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    uint Instance : SV_InstanceID;
};

StructuredBuffer<Particle> ParticleBufferGS : register(t16);

[maxvertexcount(6)]
void main(point VSOut In[1], inout TriangleStream<GSOut> output)
{
    GSOut Out[4] = { (GSOut) 0.f, (GSOut) 0.f, (GSOut) 0.f, (GSOut) 0.f };
    
    if (0 == ParticleBufferGS[In[0].Instance].active)
        return;
   
        
    float3 vWorldPos = In[0].LocalPosition.xyz + ParticleBufferGS[In[0].Instance].position.xyz;
    
    if (simulationSpace == 0)
    {
        vWorldPos += world._41_42_43;
    }
    
    float3 vViewPos = mul(float4(vWorldPos, 1.f), view).xyz;
    float3 vScale = startSize.xyz;
    
    float3 NewPos[4] =
    {
        vViewPos - float3(-0.5f, 0.5f, 0.f)     * vScale,
        vViewPos - float3(0.5f, 0.5f, 0.f)      * vScale,
        vViewPos - float3(0.5f, -0.5f, 0.f)     * vScale,
        vViewPos - float3(-0.5f, -0.5f, 0.f)    * vScale
    };
    
    for (int i = 0; i < 4; ++i)
    {
        Out[i].Position = mul(float4(NewPos[i], 1.f), projection);
    }
    
    
    Out[0].UV = float2(0.f, 0.f);
    Out[1].UV = float2(1.f, 0.f);
    Out[2].UV = float2(1.f, 1.f);
    Out[3].UV = float2(0.f, 1.f);
       
    Out[0].Instance = In[0].Instance;
    Out[1].Instance = In[0].Instance;
    Out[2].Instance = In[0].Instance;
    Out[3].Instance = In[0].Instance;
    // 0 -- 1
    // | \  |
    // 3 -- 2
    output.Append(Out[0]);
    output.Append(Out[1]);
    output.Append(Out[2]);
    output.RestartStrip();
    
    output.Append(Out[0]);
    output.Append(Out[2]);
    output.Append(Out[3]);
    output.RestartStrip();
}