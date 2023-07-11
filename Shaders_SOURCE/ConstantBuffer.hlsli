
cbuffer Transform : register(b0)
{
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
    float4 worldPosition;
    float4 startSize;
    float4 startColor;
    float startLifeTime;
    float startSpeed;
    uint maxParticles;
    uint simulationSpace;
    float radius;
    float deltaTime;
    float elapsedTime;
}

//cbuffer NoiseCB : register(b6)
//{
//    float4 NoiseSize;
//    float noiseTime;
//}



