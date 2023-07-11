#include "globals.hlsli"


RWStructuredBuffer<Particle> ParticleBufferCS : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBufferCS : register(u1);


[numthreads(128, 1, 1)]
void main(uint3 _id : SV_DispatchThreadID)
{
    if (maxParticles <= _id.x)
        return;
        
    if (0 == ParticleBufferCS[_id.x].active)
    {
        while (0 < ParticleSharedBufferCS[0].Active)
        {
            int iOriginValue = ParticleSharedBufferCS[0].Active;
            int iExchange = iOriginValue - 1;
        
            InterlockedCompareExchange(ParticleSharedBufferCS[0].Active
            , iOriginValue, iExchange, iExchange);
        
            if (iOriginValue == iExchange)
            {
                ParticleBufferCS[_id.x].active = 1;
                break;
            }
        }
        
        if (ParticleBufferCS[_id.x].active)
        {
            // ���������� ��ġ�� ������ �����Ѵ�.
            // ���ø��� �õ��� UV �� ����Ѵ�.
            float4 vRandom = (float4) 0.f;
            
            float2 vUV = float2((float) _id.x / maxParticles, 0.5f);
            vUV.x += elapsedTime;
            vUV.y += sin((vUV.x + elapsedTime) * 3.141592f * 2.f * 10.f) * 0.5f;
            
            vRandom = float4
            (
                  GaussianBlur(vUV + float2(0.f, 0.f)).x
                , GaussianBlur(vUV + float2(0.1f, 0.f)).x
                , GaussianBlur(vUV + float2(0.2f, 0.f)).x
                , GaussianBlur(vUV + float2(0.3f, 0.f)).x
            );
            
            // ���� ������ ����
            float fTheta = vRandom.x * 3.141592f * 2.f;
            ParticleBufferCS[_id.x].position.xy = float2(cos(fTheta), sin(fTheta)) * vRandom.y * radius;
            ParticleBufferCS[_id.x].position.z = 0.f;
            ParticleBufferCS[_id.x].direction.xy 
                = -normalize(float2(ParticleBufferCS[_id.x].position.xy));
            
            if (simulationSpace)
            {
                ParticleBufferCS[_id.x].position.xyz += worldPosition.xyz;
            }
            
            ParticleBufferCS[_id.x].speed = startSpeed; 
            ParticleBufferCS[_id.x].time = 0.f;
            ParticleBufferCS[_id.x].lifeTime = startLifeTime; 
        }
    }
    else
    {
        ParticleBufferCS[_id.x].time += deltaTime;
        if (ParticleBufferCS[_id.x].lifeTime < ParticleBufferCS[_id.x].time)
        {
            ParticleBufferCS[_id.x].active = 0;
        }
        else
        {
            ParticleBufferCS[_id.x].position 
            += ParticleBufferCS[_id.x].direction * ParticleBufferCS[_id.x].speed * deltaTime;
        }
    }
}