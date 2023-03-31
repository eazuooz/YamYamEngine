
struct LightColor
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
};

struct LightAttribute
{
    LightColor color;
    float4 position;
    float4 direction;
    
    int type;
    float radius; 
    float angle;
    int padding;
};

StructuredBuffer<LightAttribute> lightsAttribute2D : register(t14);
StructuredBuffer<LightAttribute> lightsAttribute3D : register(t15);

void CalculateLight2D(in out LightColor lightColor, float3 position, int idx)
{
    
    
    if (0 == lightsAttribute2D[idx].type)
    {
        lightColor.diffuse += lightsAttribute2D[idx].color.diffuse;
    }
    else if (1 == lightsAttribute2D[idx].type)
    {
        float length = distance(position.xy, lightsAttribute2D[idx].position.xy);
        
        if (length < lightsAttribute2D[idx].radius)
        {
            float ratio = 1.0f - (length / lightsAttribute2D[idx].radius);
            lightColor.diffuse += lightsAttribute2D[idx].color.diffuse * ratio;
        }
    }
    else
    {
        
    }

}