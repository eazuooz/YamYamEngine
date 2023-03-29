#include "globals.hlsli"
RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);

[numthreads(128, 1, 1)]
void main(uint3 _id : SV_DispatchThreadID)
{
    if (elementCount <= _id.x)
        return;
        
    //ParticleBuffer[_id.x].position += ParticleBuffer[_id.x].direction 
    //    * ParticleBuffer[_id.x].speed * deltaTime;
    
    
    
    if (0 == ParticleBuffer[_id.x].active)
    {
        while (0 < ParticleSharedBuffer[0].gActiveCount)
        {
            int iOriginValue = ParticleSharedBuffer[0].gActiveCount;
            int iExchange = iOriginValue - 1;
        
            InterlockedExchange(ParticleSharedBuffer[0].gActiveCount
            , iExchange, iExchange);
        
            if (iOriginValue == iExchange)
            {
                ParticleBuffer[_id.x].active = 1;
                break;
            }
        }
    }
    else
    {
        ParticleBuffer[_id.x].position += ParticleBuffer[_id.x].direction 
        * ParticleBuffer[_id.x].speed * deltaTime;
    }
}