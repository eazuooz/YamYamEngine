#include "globals.hlsli"

struct KeyFrame
{
    //Matrix rootTransform;
    float4 translate;
    float4 scale;
    float4 quarternion;
    //double time;
    //int frame;
};
StructuredBuffer<KeyFrame> keyFrames : register(t17);
StructuredBuffer<matrix> offsetMatrices : register(t18);
RWStructuredBuffer<matrix> rwBoneMatrices : register(u2);
float4 CompareVector_GetMaxMin(float4 q1, float4 q2)
{
    float4 temp =
    {
        (q1[0] < q2[0]) ? asfloat((uint) 0xffffffff) : 0.0f,
        (q1[1] < q2[1]) ? asfloat((uint) 0xffffffff) : 0.0f,
        (q1[2] < q2[2]) ? asfloat((uint) 0xffffffff) : 0.0f,
        (q1[3] < q2[3]) ? asfloat((uint) 0xffffffff) : 0.0f
    };

    return temp;
}
float4 PermuteVector(uint x, uint y, uint z, uint w, float4 v1,float4 v2)
{
    float4 arr[2] = { v1, v2 };
    float4 temp = (float4) 0.f;

    uint ix = x & 3;
    uint iy = x >> 2;
    temp[0] = arr[iy][ix];

    ix = y & 3;
    iy = y >> 2;
    temp[1] = arr[iy][ix];

    ix = z & 3;
    iy = z >> 2;
    temp[2] = arr[iy][ix];

    ix = w & 3;
    iy = w >> 2;
    temp[3] = arr[iy][ix];;

    return temp;
}
float4 VectorShiftLeft(in float4 v1, in float4 v2, uint step)
{
    return PermuteVector(step, ((step) + 1), ((step) + 2), ((step) + 3), v1, v2);
}
float4 SelectBits(float4 q1, float4 q2, float4 flags)
{
    uint4 iq1 = asuint(q1);
    uint4 iq2 = asuint(q2);
    uint4 iflags = asuint(flags);

    int4 temp =
    {
        (iq1[0] & ~iflags[0]) | (iq2[0] & iflags[0]),
        (iq1[1] & ~iflags[1]) | (iq2[1] & iflags[1]),
        (iq1[2] & ~iflags[2]) | (iq2[2] & iflags[2]),
        (iq1[3] & ~iflags[3]) | (iq2[3] & iflags[3]),
    };

    return asfloat(temp);
}
float4 CalculateXOR(float4 v1)
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
matrix CreateMatrixForQuaterion(float4 quat)
{
    float4 Constant1110 = float4(1.f, 1.f, 1.f, 0.f);

    float4 Q0 = quat + quat;
    float4 Q1 = quat * Q0;

    float4 V0 = PermuteVector(1, 0, 0, 7, Q1, Constant1110);
    float4 V1 = PermuteVector(2, 2, 1, 7, Q1, Constant1110);

    float4 R0 = Constant1110 - V0;
    R0 = R0 - V1;

    V0 = float4(quat[0], quat[0], quat[1], quat[3]);
    V1 = float4(Q0[2], Q0[1], Q0[2], Q0[3]);
    V0 = V0 * V1;

    V1 = float4(quat.w, quat.w, quat.w, quat.w);
    float4 V2 = float4(Q0[1], Q0[2], Q0[0], Q0[3]);
    V1 = V1 * V2;

    float4 R1 = V0 + V1;
    float4 R2 = V0 - V1;

    V0 = PermuteVector(1, 4, 5, 2, R1, R2);
    V1 = PermuteVector(0, 6, 0, 6, R1, R2);

    matrix M = (matrix) 0.0f;
    M._11_12_13_14 = PermuteVector(0, 4, 5, 3, R0, V0);
    M._21_22_23_24 = PermuteVector(6, 1, 7, 3, R0, V0);
    M._31_32_33_34 = PermuteVector(4, 5, 2, 3, R0, V1);
    M._41_42_43_44 = float4(0.f, 0.f, 0.f, 1.f);
    return M;
}
matrix CreateTransformMatrix(float4 scale, float4 quaternion, float4 translate)
{
    matrix M = (matrix) 0.0f;;
    M._11_22_33 = scale.xyz;
    
    matrix rot = CreateMatrixForQuaterion(quaternion);
    M = mul(M, rot);
    
    float3 pos = translate.xyz;
    M._41_42_43 += pos;
    
    return M;
}
float4 LerpQuaternion(in float4 q1, in float4 q2, float ratio)
{
    float4 vT = float4(ratio, ratio, ratio, ratio);
    const float4 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };

    float dotQ1Q2 = dot(q1, q2);
    float4 CosOmega = float4(dotQ1Q2, dotQ1Q2, dotQ1Q2, dotQ1Q2);

    const float4 Zero = (float4) 0.f;
    float4 Control = CompareVector_GetMaxMin(CosOmega, Zero);
    float4 Sign = SelectBits(float4(1.f, 1.f, 1.f, 1.f), float4(-1.f, -1.f, -1.f, -1.f), Control);

    CosOmega = CosOmega * Sign;
    Control = CompareVector_GetMaxMin(CosOmega, OneMinusEpsilon);

    float4 SinOmega = float4(1.f, 1.f, 1.f, 1.f) - (CosOmega * CosOmega);
    SinOmega = float4(sqrt(SinOmega.x), sqrt(SinOmega.y), sqrt(SinOmega.z), sqrt(SinOmega.w));

    float4 Omega = float4(atan2(SinOmega.x, CosOmega.x)
        , atan2(SinOmega.y, CosOmega.y)
        , atan2(SinOmega.z, CosOmega.z)
        , atan2(SinOmega.w, CosOmega.w));

    float maxValue_32bit = 0x80000000U;
    float4 SignMask = float4(asfloat(maxValue_32bit), asfloat(maxValue_32bit), asfloat(maxValue_32bit), asfloat(maxValue_32bit));
    float4 V01 = VectorShiftLeft(vT, Zero, 2);
    SignMask = VectorShiftLeft(SignMask, Zero, 3);

    V01 = CalculateXOR(V01/*, SignMask*/);
    V01 = float4(1.0f, 0.0f, 0.0f, 0.0f) + V01;

    float4 InvSinOmega = float4(1.f, 1.f, 1.f, 1.f) / SinOmega;

    float4 S0 = V01 * Omega;
    S0 = float4(sin(S0.x), sin(S0.y), sin(S0.z), sin(S0.w));
    S0 = S0 * InvSinOmega;
    S0 = SelectBits(V01, S0, Control);

    float4 S1 = float4(S0.y, S0.y, S0.y, S0.y);
    S0 = float4(S0.x, S0.x, S0.x, S0.x);

    S1 = S1 * Sign;

    float4 Result = q1 * S0;
    Result = (q2 * S1) + Result;

    return Result;
}

[numthreads(256, 1, 1)]
void main(int3 DTid : SV_DispatchThreadID)
{
    if (boneCount <= DTid.x)
        return;

    uint currentFrame = boneCount * frameIdx + DTid.x;
    uint nextFrame = boneCount * (frameIdx + 1) + DTid.x;

    float4 scale = lerp(keyFrames[currentFrame].scale, keyFrames[nextFrame].scale, frameRatio);
    float4 translate = lerp(keyFrames[currentFrame].translate, keyFrames[nextFrame].translate, frameRatio);
    float4 quternion = LerpQuaternion(keyFrames[currentFrame].quarternion, keyFrames[nextFrame].quarternion, frameRatio);

    matrix boneMatrix = CreateTransformMatrix(scale, quternion, translate);
    matrix offsetMatrix = transpose(offsetMatrices[DTid.x]);
    
    rwBoneMatrices[DTid.x] = mul(offsetMatrix, boneMatrix);
}