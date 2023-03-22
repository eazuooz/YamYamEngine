#include "globals.hlsli"

RWTexture2D<float4> tex : register(u0);

// SV_GroupID           : �����尡 ���� �׷��� ��ǥ
// SV_GoupThreadID      : �׷� ������, �������� ��ǥ
// SV_GoupIndex         : �׷� ������, �������� �ε��� (1����)
// SV_DispatchThreadID  : ��ü ������(��� �׷� ����) ��������, ȣ��� �������� ��ǥ

[numthreads(32, 32, 1)] // �׷� �� ������ ���� (�ִ� 1024 ���� ���� ����)
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (1024 <= DTid.x || 1024 <= DTid.y)
    {
        return;
    }
    
    tex[DTid.xy] = float4(1.f, 0.f, 0.f, 1.f);
    
    //if ((uint) resolution.x <= DTid.x || (uint) resolution.y <= DTid.y)
    //{
    //    return;
    //}
    
    //tex[DTid.xy] = particleColor;
}
