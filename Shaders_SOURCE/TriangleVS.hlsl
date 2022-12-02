
struct VSIn
{
    float3 Pos : POSITIONT;
    float4 Color : COLOR;
};

struct VSOut
{
    float3 Pos : SV_Position;
    float4 Color : COLOR;
};

float4 main( VSIn In ) 
{
    VSOut Out = (VSOut)0.0f;
    
    Out.Pos = float4(In.Pos, 1.0f);
    Out.Color = In.Color;
    
	return Out;
}