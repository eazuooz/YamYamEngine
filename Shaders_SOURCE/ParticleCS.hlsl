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
        
            //InterlockedExchange(ParticleSharedBufferCS[0].gActiveCount
            //, iExchange, iExchange);
            InterlockedCompareExchange(ParticleSharedBufferCS[0].gActiveCount
            , iOriginValue, iExchange, iExchange);
        
            if (iOriginValue == iExchange)
            {
                ParticleBufferCS[_id.x].active = 1;
                break;
            }
        }
        
        if (ParticleBufferCS[_id.x].active)
        {
            // 랜덤값으로 위치와 방향을 설정한다.
            // 샘플링을 시도할 UV 를 계산한다.
            float3 vRandom = (float3) 0.f;
            
            float2 vUV = float2((float) _id.x / elementCount, 0.5f);
            vUV.x += elapsedTime;
            vUV.y += sin((vUV.x + elapsedTime) * 3.141592f * 2.f * 10.f) * 0.5f;
            
            vRandom = float3
            (
                  GaussianBlur(vUV + float2(0.f, 0.f)).x
                , GaussianBlur(vUV + float2(0.1f, 0.f)).x
                , GaussianBlur(vUV + float2(0.2f, 0.f)).x
            );
            
            ParticleBufferCS[_id.x].position.xyz = vRandom.xyz * 1000.0f - 500.0f;
            ParticleBufferCS[_id.x].position.z = 100.f;
        }
    }
    else
    {
        //ParticleBufferCS[_id.x].position += ParticleBufferCS[_id.x].direction 
        //* ParticleBufferCS[_id.x].speed * deltaTime;
    }
}