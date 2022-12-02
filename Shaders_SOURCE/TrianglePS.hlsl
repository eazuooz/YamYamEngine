struct VSOut
{
    float3 Pos : SV_Position;
    float4 Color : COLOR;
};

float4 main(VSOut Out) : SV_TARGET
{
    float4 OutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return OutColor;
}