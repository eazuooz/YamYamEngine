#include "globals.hlsli"

RWStructuredBuffer<Particle> ParticleBufferCS : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBufferCS : register(u1);

[numthreads(128, 1, 1)]
void main(uint3 _id : SV_DispatchThreadID)
{
    if (elementCount <= _id.x)
        return;
        
    if (0 == ParticleBufferCS[_id.x].active)
    {
        while (0 < ParticleSharedBufferCS[0].gActiveCount)
        {
            int iOriginValue = ParticleSharedBufferCS[0].gActiveCount;
            int iExchange = iOriginValue - 1;
        
            InterlockedExchange(ParticleSharedBufferCS[0].gActiveCount
            , iExchange, iExchange);
        
            if (iOriginValue == iExchange)
            {
                ParticleBufferCS[_id.x].active = 1;
                break;
            }
        }
    }
    else
    {
        ParticleBufferCS[_id.x].position += ParticleBufferCS[_id.x].direction 
        * ParticleBufferCS[_id.x].speed * deltaTime;
    }
}