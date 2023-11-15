// Texture
Texture2D albedo : register(t0);
//Texture2D tex_1 : register(t1);
//Texture2D tex_2 : register(t2);
//Texture2D tex_3 : register(t3);
//Texture2D tex_4 : register(t4);
//Texture2D tex_5 : register(t5);
//Texture2D tex_6 : register(t6);
//Texture2D tex_7 : register(t7);

// Texture Cube
TextureCube cubeMap : register(t8);

//TextureCube g_texcube_0 : register(t8);
//TextureCube g_texcube_1 : register(t9);

// Texture2DArray 
//Texture2DArray g_texarr_0 : register(t10);
//Texture2DArray g_texarr_1 : register(t11);

// Sprite Atlas Texture
Texture2D spriteAtlas : register(t12);

// Noise Texture
//Texture2D NoiseTexture : register(t13);

// Structed Buffer
//StructuredBuffer<LightAttribute> lightsAttribute2D : register(t14);
//StructuredBuffer<LightAttribute> lightsAttribute3D : register(t15);
//StructuredBuffer<Particle> ParticleBufferGS : register(t16);
//StructuredBuffer<KeyFrame> keyFrames : register(t17);
//StructuredBuffer<matrix> offsetMatrices : register(t18);



// RenderTarget CopyTex
//Texture2D postProcessing : register(t60);

//msaa
//Texture2DMS<float4, 4> postProcessing : register(t60);
//Texture2DMS<float4, 4> imguiGameView : register(t61);

Texture2D postProcessing : register(t60);
Texture2D imguiGameView : register(t61);

//
//RWStructuredBuffer<Particle> ParticleBufferCS : register(u0);
//RWStructuredBuffer<ParticleShared> ParticleSharedBufferCS : register(u1);
//RWStructuredBuffer<matrix> globalMatrices : register(u2);

