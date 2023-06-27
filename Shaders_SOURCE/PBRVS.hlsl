#include "globals.hlsli"

struct VS_Input
{
    float3 posModel : POSITION; //�� ��ǥ���� ��ġ position
    float3 normalModel : NORMAL0; // �� ��ǥ���� normal    
    float2 texcoord : TEXCOORD0;
    float3 tangentModel : TANGENT0;
};

struct PS_Input
{
    float4 posProj : SV_POSITION; // Screen position
    float3 posWorld : POSITION; // World position (���� ��꿡 ���)
    float3 normalWorld : NORMAL0;
    float2 texcoord : TEXCOORD0;
    float3 tangentWorld : TANGENT0;
    float3 color : COLOR; // Normal lines ���̴����� ���
};

// Vertex Shader������ �ؽ��� ���
Texture2D g_heightTexture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer BasicVertexConstantData : register(b0)
{
    matrix modelWorld;
    matrix invTranspose;
    matrix view;
    matrix projection;
    int useHeightMap;
    float heightScale;
    float2 dummy;
};

PS_Input main(VS_Input input)
{
    // ��(Model) ����� �� �ڽ��� �������� 
    // ���� ��ǥ�迡���� ��ġ�� ��ȯ�� �����ݴϴ�.
    // �� ��ǥ���� ��ġ -> [�� ��� ���ϱ�] -> ���� ��ǥ���� ��ġ
    // -> [�� ��� ���ϱ�] -> �� ��ǥ���� ��ġ -> [�������� ��� ���ϱ�]
    // -> ��ũ�� ��ǥ���� ��ġ
    
    // �� ��ǥ��� NDC�̱� ������ ���� ��ǥ�� �̿��ؼ� ���� ���
    
    PS_Input output;
    
    // Normal ���� ���� ��ȯ (Height Mapping)
    float4 normal = float4(input.normalModel, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    
    // Tangent ���͵� ���� ��ȯ
    float4 tangentWorld = float4(input.tangentModel, 0.0f);
    tangentWorld = mul(tangentWorld, invTranspose);

    float4 pos = float4(input.posModel, 1.0f);
    pos = mul(pos, modelWorld);
    
    if (useHeightMap)
    {
        // VertexShader������ SampleLevel ���
        // Heightmap�� ���� ����̶� �������� .r�� float �ϳ��� ���
        float height = g_heightTexture.SampleLevel(g_sampler, input.texcoord, 0).r;
        height = height * 2.0 - 1.0;
        pos += float4(output.normalWorld * height * heightScale, 0.0);
    }

    output.posWorld = pos.xyz; // ���� ��ġ ���� ����

    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.posProj = pos;
    output.texcoord = input.texcoord;
    output.tangentWorld = tangentWorld.xyz;

    output.color = float3(0.0f, 0.0f, 0.0f);

    return output;
}