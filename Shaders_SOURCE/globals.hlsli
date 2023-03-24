#include "Light.hlsli"
#include "Particle.hlsli"

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

cbuffer Animation2D : register(b3)
{
    float2 leftTop;
    float2 spriteSize;
    float2 offset;
    float2 atlasSize;

    uint animationType;
};



cbuffer NumberOfLight : register(b4)
{
    uint numberOfLight;
}

cbuffer ParticleSystem : register(b5)
{
    float4 particleColor;
    uint elementCount;
    float deltaTime;
}

// Texture
Texture2D triangleTexture : register(t0);

// Atlas Texture
Texture2D atlasTexture : register(t12);

StructuredBuffer<LightAttribute> lightsAttribute : register(t13);
StructuredBuffer<Particle> particleBuffer : register(t15);


SamplerState anisotropicSampler : register(s0); // anisotropic sampler
SamplerState pointSampler : register(s1); // point sampler



//Func

void CalculateLight2D(in out LightColor lightColor, float3 position, int idx)
{
    
    
    if (0 == lightsAttribute[idx].type)
    {
        lightColor.diffuse += lightsAttribute[idx].color.diffuse;
    }
    else if (1 == lightsAttribute[idx].type)
    {
        float length = distance(position.xy, lightsAttribute[idx].position.xy);
        
        if (length < lightsAttribute[idx].radius)
        {
            float ratio = 1.0f - (length / lightsAttribute[idx].radius);
            lightColor.diffuse += lightsAttribute[idx].color.diffuse * ratio;
        }
    }
    else
    {
        
    }

}