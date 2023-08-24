
#define DIRECTIONAL_LIGHT   0
#define POINT_LIGHT         1
#define SPOT_LIGHT          2

struct LightAttribute
{
    float4 color;
    float4 position;
    float4 direction;
    
    int type;
    float radius; 
    float angle;
    float power;
};

StructuredBuffer<LightAttribute> lights : register(t14);

void CalculateLight2D(in out float4 lightColor, float3 position, int idx)
{
    if (0 == lights[idx].type)
    {
        lightColor += lights[idx].color;
    }
    else if (1 == lights[idx].type)
    {
        float length = distance(position.xy, lights[idx].position.xy);
        
        if (length < lights[idx].radius)
        {
            float ratio = 1.0f - (length / lights[idx].radius);
            lightColor += lights[idx].color * ratio;
        }
    }
    else
    {
        
    }

}