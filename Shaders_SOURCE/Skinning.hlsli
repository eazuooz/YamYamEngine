

struct SkinningInfo
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

StructuredBuffer<matrix> BoneMatrix : register(t30);

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return BoneMatrix[(boneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices, int _iRowIdx)
{
    SkinningInfo info = (SkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;
        
        matrix matBone = (matrix) 0.0f;
        matBone  = GetBoneMat((int) _vIndices[i], _iRowIdx);
        //matBone._11_12_13_14 = float4(1.0f, 0.0f, 0.0f, 0.0f);
        //matBone._21_22_23_24 = float4(0.0f, 1.0f, 0.0f, 0.0f);
        //matBone._31_32_33_34 = float4(0.0f, 0.0f, 1.0f, 0.0f);
        //matBone._41_42_43_44 = float4(0.0f, 0.0f, 0.0f, 1.0f);
        
        //float4 pos = mul(float4(_vPos, 1.f), matBone);
        //pos *= _vWeight[i];
        
        info.Pos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        //info.Pos += pos.xyz;
        //info.Tangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        //info.BiNormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.Normal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }
    
    //matrix matBone = (matrix) 0.0f;
    //matBone = BoneMatrix[100];
    //matBone._11_12_13_14 = float4(1.0f, 0.0f, 0.0f, 0.0f);
    //matBone._21_22_23_24 = float4(0.0f, 1.0f, 0.0f, 0.0f);
    //matBone._31_32_33_34 = float4(0.0f, 0.0f, 1.0f, 0.0f);
    //matBone._41_42_43_44 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    //info.Pos += (mul(float4(_vPos, 1.f), matBone) /** _vWeight[i]*/).xyz;

    _vPos = info.Pos;
    _vTangent = normalize(info.Tangent);
    _vBinormal = normalize(info.BiNormal);
    _vNormal = normalize(info.Normal);
}



