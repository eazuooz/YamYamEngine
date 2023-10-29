

struct Vertex
{
    float3 Pos;
    float3 Tangent;
    float3 BiNormal;
    float3 Normal;
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

StructuredBuffer<matrix> boneMatrices : register(t30);

matrix GetBoneMatrix(int boneIndex, int row)
{
    int index = (boneCount * row) + boneIndex;
    return boneMatrices[index];
}

void Skinning(inout float3 pos, inout float3 tangent, inout float3 biNormal, inout float3 normal
    , inout float4 weights, inout float4 indices, int row)
{
    Vertex vtx = (Vertex) 0.0f;

    if (row == -1)
        return;

    [Roll]
    for (int i = 0; i < 4; ++i)
    {
        if (0.f == weights[i])
            continue;
        
        matrix matBone = (matrix) 0.0f;
        matBone = GetBoneMatrix((int) indices[i], row);
        
        vtx.Pos += (mul(float4(pos, 1.f), matBone) * weights[i]).xyz;
        vtx.Tangent += (mul(float4(tangent, 0.f), matBone) * weights[i]).xyz;
        vtx.BiNormal += (mul(float4(biNormal, 0.f), matBone) * weights[i]).xyz;
        vtx.Normal += (mul(float4(normal, 0.f), matBone) * weights[i]).xyz;
    }
    
    pos = vtx.Pos;
    tangent = normalize(vtx.Tangent);
    biNormal = normalize(vtx.BiNormal);
    normal = normalize(vtx.Normal);
}



