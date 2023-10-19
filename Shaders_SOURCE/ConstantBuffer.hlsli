#include "Material.hlsli"

cbuffer Transform : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
};

cbuffer Material : register(b1)
{
    //float4 diffuse;
    //float4 ambient;
    //float4 specular;
    
    //float shininess;
    //int usedAlbedo;
    //float dummy2;
    //float dummy3;
    Material mat;
};

cbuffer Camera : register(b2)
{
    float4 cameraPosition;
    float2 cameraScale;
    float2 resolution;
};

cbuffer Animation2D : register(b3)
{
    //type
    uint animationType;
    int padd1;
    int padd2;
    int padd3;
    
    //2D
    float2 leftTop;
    float2 spriteSize;
    float2 offset;
    float2 atlasSize;

    //3D
    uint boneCount;
    uint frameIdx;
    uint nextFrameIdx;
    float frameRatio;
};

cbuffer Light : register(b4)
{
    uint lightCount;
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



