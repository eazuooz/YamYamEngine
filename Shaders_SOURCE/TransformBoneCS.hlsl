#include "globals.hlsli"

struct KeyFrame
{
    float4 Translate;
    float4 Scale;
    float4 Quaternion;
};

StructuredBuffer<KeyFrame> keyFrames : register(t17);
StructuredBuffer<matrix> offsetMatrices : register(t18);

RWStructuredBuffer<matrix> globalMatrices : register(u2);

//cbuffer Animation2D : register(b3)
//{
//    uint boneCount;
//    uint frameIdx;
//    uint nextFrameIdx;
//    float frameRatio;
//};

float4 VectorLess(float4 v1, float4 v2)
{
    float4 temp =
    {
        (v1.x < v2.x) ? asfloat( (uint) 0xffffffff ) : 0.0f,
        (v1.y < v2.y) ? asfloat( (uint) 0xffffffff ) : 0.0f,
        (v1.z < v2.z) ? asfloat( (uint) 0xffffffff ) : 0.0f,
        (v1.w < v2.w) ? asfloat((uint) 0xffffffff) : 0.0f
    };

    return temp;
}
float4 VectorPermute(uint x, uint y, uint z, uint w, float4 v1, float4 v2)
{
    float4 buff[2] = { v1, v2 };
    float4 temp = (float4)0.0f;

    const uint ix = 0;
    const uint iy = 0;
    
    ix = x & 3;
    iy = x >> 2;
    temp.x = buff[iy][ix];

    ix = y & 3;
    iy = y >> 2;
    temp.y = buff[iy][ix];

    ix = z & 3;
    iy = z >> 2;
    temp.z = buff[iy][ix];

    ix = w & 3;
    iy = w >> 2;
    temp.w = buff[iy][ix];

    return temp;
}
float4 VectorShiftLeft(float4 v1, float4 v2, uint value)
{
    return VectorPermute(value, ((value) + 1), ((value) + 2), ((value) + 3), v1, v2);
}
float4 VectorSelect(float4 v1, float4 v2, float4 flag)
{
    uint4 iv1 = asuint(v1);
    uint4 iv2 = asuint(v2);
    uint4 iflag = asuint(flag);

    int4 temp =
    {
        (iv1[0] & ~iflag[0]) | (iv2[0] & iflag[0]),
        (iv1[1] & ~iflag[1]) | (iv2[1] & iflag[1]),
        (iv1[2] & ~iflag[2]) | (iv2[2] & iflag[2]),
        (iv1[3] & ~iflag[3]) | (iv2[3] & iflag[3]),
    };

    return asfloat(temp);
}
float4 VectorXorInt(float4 v1, float4 v2)
{
    uint4 iv1 = { asuint(v1.x), asuint(v1.y), asuint(v1.z), asuint(v1.w) };
    uint4 iv2 = { 2147483648, 0, 0, 0 };

    uint4 temp =
    {
        iv1[0] ^ iv2[0],
        iv1[1] ^ iv2[1],
        iv1[2] ^ iv2[2],
        iv1[3] ^ iv2[3]
    };

    return float4(asfloat(temp.x), asfloat(temp.y), asfloat(temp.z), asfloat(temp.w));
}
matrix MatrixRotationQuaternion(float4 quaternion)
{
    float4 q0 = quaternion + quaternion;
    float4 q1 = quaternion * q0;

    float4 V0 = VectorPermute(1, 0, 0, 7, q1, float4(1.f, 1.f, 1.f, 0.f));
    float4 V1 = VectorPermute(2, 2, 1, 7, q1, float4(1.f, 1.f, 1.f, 0.f));
    
    float4 R0 = float4(1.f, 1.f, 1.f, 0.f) - V0;
    R0 = R0 - V1;

    V0 = float4(quaternion[0], quaternion[0], quaternion[1], quaternion[3]);
    V1 = float4(q0[2], q0[1], q0[2], q0[3]);
    V0 = V0 * V1;

    V1 = float4(quaternion.w, quaternion.w, quaternion.w, quaternion.w);
    float4 V2 = float4(q0[1], q0[2], q0[0], q0[3]);
    V1 = V1 * V2;

    float4 R1 = V0 + V1;
    float4 R2 = V0 - V1;

    V0 = VectorPermute(1, 4, 5, 2, R1, R2);
    V1 = VectorPermute(0, 6, 0, 6, R1, R2);

    matrix temp = (matrix) 0.f;
    temp._11_12_13_14 = VectorPermute(0, 4, 5, 3, R0, V0);
    temp._21_22_23_24 = VectorPermute(6, 1, 7, 3, R0, V0);
    temp._31_32_33_34 = VectorPermute(4, 5, 2, 3, R0, V1);
    temp._41_42_43_44 = float4(0.f, 0.f, 0.f, 1.f);
    return temp;
}
matrix MatrixAffineTransformation(float4 origin, float4 scale, float4 quaternion, float4 translate)
{
    matrix scl = (matrix) 0.f;
    scl._11_22_33 = scale.xyz;

    float4 org = float4(origin.xyz, 0.f);
    float4 trs = float4(translate.xyz, 0.f);

    matrix rot = MatrixRotationQuaternion(quaternion);

    matrix temp = scl;
    temp._41_42_43_44 = temp._41_42_43_44 - org;
    temp = mul(temp, rot);
    temp._41_42_43_44 = temp._41_42_43_44 + org;
    temp._41_42_43_44 = temp._41_42_43_44 + trs;
    return temp;
}
float4 QuternionLerp(in float4 q1, in float4 q2, float ratio)
{
    float4 vT = float4(ratio, ratio, ratio, ratio);

    // Result = Q1 * sin((1.0 - t) * Omega) / sin(Omega) + Q2 * sin(t * Omega) / sin(Omega)
    const float4 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };

    float fQDot = dot(q1, q2);
    float4 CosOmega = float4(fQDot, fQDot, fQDot, fQDot);

    const float4 Zero = (float4) 0.f;
    float4 Control = VectorLess(CosOmega, Zero);
    float4 Sign = VectorSelect(float4(1.f, 1.f, 1.f, 1.f), float4(-1.f, -1.f, -1.f, -1.f), Control);

    CosOmega = CosOmega * Sign;
    Control = VectorLess(CosOmega, OneMinusEpsilon);

    float4 SinOmega = float4(1.f, 1.f, 1.f, 1.f) - (CosOmega * CosOmega);
    SinOmega = float4(sqrt(SinOmega.x), sqrt(SinOmega.y), sqrt(SinOmega.z), sqrt(SinOmega.w));

    float4 Omega = float4(atan2(SinOmega.x, CosOmega.x)
        , atan2(SinOmega.y, CosOmega.y)
        , atan2(SinOmega.z, CosOmega.z)
        , atan2(SinOmega.w, CosOmega.w));

    float4 SignMask = float4(asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U));
    float4 V01 = VectorShiftLeft(vT, Zero, 2);
    SignMask = VectorShiftLeft(SignMask, Zero, 3);

    V01 = VectorXorInt(V01, SignMask);
    V01 = float4(1.0f, 0.0f, 0.0f, 0.0f) + V01;

    float4 InvSinOmega = float4(1.f, 1.f, 1.f, 1.f) / SinOmega;

    float4 S0 = V01 * Omega;
    S0 = float4(sin(S0.x), sin(S0.y), sin(S0.z), sin(S0.w));
    S0 = S0 * InvSinOmega;
    S0 = VectorSelect(V01, S0, Control);

    float4 S1 = float4(S0.y, S0.y, S0.y, S0.y);
    S0 = float4(S0.x, S0.x, S0.x, S0.x);

    S1 = S1 * Sign;

    float4 Result = q1 * S0;
    Result = (q2 * S1) + Result;

    return Result;
}

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (boneCount <= DTid.x)
        return;
    
    matrix matOffset = transpose(offsetMatrices[DTid.x]);
    matrix transform = (matrix) 0.0f;
    
    uint currentIdx = boneCount * frameIdx + DTid.x;
    uint nextIdx = boneCount * (frameIdx + 1) + DTid.x;

    float4 scale = lerp(keyFrames[currentIdx].Scale, keyFrames[nextIdx].Scale, frameRatio);
    float4 translate = lerp(keyFrames[currentIdx].Translate, keyFrames[nextIdx].Translate, frameRatio);
    float4 quaternion = QuternionLerp(keyFrames[currentIdx].Quaternion, keyFrames[nextIdx].Quaternion, frameRatio);

    transform = MatrixAffineTransformation(float4(0.0f, 0.0f, 0.0f, 1.0f), scale, quaternion, translate);
    
    globalMatrices[DTid.x] = mul(matOffset, transform);

}