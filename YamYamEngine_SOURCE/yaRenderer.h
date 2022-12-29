#pragma once
#include "yaEnums.h"
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaMath.h"
#include "yaGraphicsDevice_DX11.h"

#include "yaMesh.h"
#include "yaShader.h"
#include "yaConstantBuffer.h"

using namespace ya::graphics;
using namespace ya::enums;
using namespace ya::math;

namespace ya::renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
		Vector2 uv;
	};

	extern Mesh* mesh;
	extern Shader* shader;
	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];

	// Initialize the renderer
	void Initialize();
	void Release();
}
