
struct Material
{
    float4 diffuseColor;
    float4 ambientColor;
    float4 specularColor;
    
    float shininess;
    int usedAlbedo;
    int usedSpecular;
    int usedNormal;
};
