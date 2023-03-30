
Texture2D NoiseTexture : register(t16);


static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

//float4 GaussianBlur(float2 UV)
//{
//    float4 vOutColor = (float4) 0.f;
    
//    if (1.0f < UV.x)
//        UV.x = frac(UV.x);
//    else if (UV.x < 0.f)
//        UV.x = 1.0f + frac(UV.x);
    
//    if (1.0f < UV.y)
//        UV.y = frac(UV.y);
//    else if (UV.y < 0.f)
//        UV.x = 1.0f + frac(UV.x);
    
//    int2 iUV = UV * g_vNoiseResolution;
//    iUV -= int2(2, 2);
    
//    for (int j = 0; j < 5; ++j)
//    {
//        for (int i = 0; i < 5; ++i)
//        {
//            int2 idx = int2(iUV.y + i, iUV.x + j);
//            vOutColor += g_Noise[idx] * GaussianFilter[j][i];
//        }
//    }
    
//    return vOutColor;
//}