#include "globals.hlsli"
RWStructuredBuffer<Particle> ParticleBuffer : register(u0);

[numthreads(128, 1, 1)]
void main(uint3 _id : SV_DispatchThreadID)
{
    if (elementCount <= _id.x)
        return;
        
    ParticleBuffer[_id.x].position += ParticleBuffer[_id.x].direction 
    * ParticleBuffer[_id.x].speed * deltaTime;
    //    += ParticleBuffer[_id.x].direction * ParticleBuffer[_id.x].speed /** deltaTime*/;
}