#pragma once
#include "yaEnums.h"
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaMath.h"
#include "yaGraphicsDevice_DX11.h"


using namespace ya::graphics;
using namespace ya::enums;
using namespace ya::math;

namespace ya::renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[3];
	extern ID3D11Buffer* triangleVertexBuffer;
	extern ID3D11Buffer* triangleConstantBuffer;
	extern ID3D11Buffer* triangleIndexBuffer;

	extern ID3DBlob* errorBlob;
	
	extern ID3DBlob* triangleVSBlob;
	extern ID3D11VertexShader* triangleVSShader;
	
	extern ID3DBlob* trianglePSBlob;
	extern ID3D11PixelShader* trianglePSShader;

	extern ID3D11InputLayout* triangleLayout;

	//Shader				shaders[SHADERTYPE_COUNT];
	//Texture				textures[TEXTYPE_COUNT];
	//InputLayout			inputLayouts[ILTYPE_COUNT];
	//RasterizerState		rasterizers[RSTYPE_COUNT];
	//DepthStencilState	depthStencils[DSSTYPE_COUNT];
	//BlendState			blendStates[BSTYPE_COUNT];
	//GPUBuffer			constantBuffers[CBTYPE_COUNT];
	//GPUBuffer			resourceBuffers[RBTYPE_COUNT];
	//Sampler				samplers[SAMPLER_COUNT];



	// Initialize the renderer
	void Initialize();
	void Release();
}
