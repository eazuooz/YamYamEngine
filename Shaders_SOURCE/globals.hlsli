

cbuffer Transform : register(b0)
{
    //float4 cbPos;
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
};

cbuffer Material : register(b1)
{
    int int_0;
    float float_0;
    float2 vec2_0;
    float3 vec3_0;
    float4 vec4_3;
    matrix mat_0;
};

cbuffer Grid : register(b2)
{
    float4 cameraPosition;
    float2 cameraScale;
    float2 resolution;
};

Texture2D triangleTexture : register(t0);

SamplerState anisotropicSampler : register(s0); // anisotropic sampler
SamplerState pointSampler : register(s1); // point sampler