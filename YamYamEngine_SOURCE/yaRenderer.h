#pragma once
#include "yaEnums.h"
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaMath.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaConstantBuffer.h"

#include "yaMesh.h"
#include "yaShader.h"
//#include "yaMaterial.h"

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

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Vector4 pos;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		int iNum;
		float fNum;
		Vector2 vector2;
		Vector3 vector3;
		Vector4 vector4;
		Matrix matrix;
	};

	extern std::shared_ptr < Mesh> mesh;
	extern std::shared_ptr<Shader> shader;

	//extern Mesh* mesh;
	//extern Shader* shader;
	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];

	// Initialize the renderer
	void Initialize();
	void Release();
}
