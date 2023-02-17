#pragma once
#include "yaEnums.h"
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaMath.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaConstantBuffer.h"
#include "yaCamera.h"



using namespace ya::graphics;
using namespace ya::enums;
using namespace ya::math;

namespace ya::renderer
{
	struct Vertex
	{
		Vector4 pos;
		Vector4 color;
		Vector2 uv;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix mWorld;
		Matrix mView;
		Matrix mProjection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		int iNum;
		float fNum;
		Vector2 vector2;
		Vector3 vector3;
		Vector4 vector4;
		Matrix matrix;

		//Vector2 resolution;
		//Vector4 cameraPosition;
		//Vector2 cameraScale;
	};

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 cameraPosition;
		Vector2 cameraScale;
		Vector2 resolution;
	};

	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizeStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStateStates[];

	extern std::vector<Camera*> cameras;
	extern std::vector<DebugMesh> debugMeshes;
	extern Camera* mainCamera;

	// Initialize the renderer
	void Initialize();
	void Render();
	void DebugRender();
	void Release();
	void LoadMesh();
	void LoadMaterial();

	// Camera
	void PushDebugMesh(DebugMesh& mesh);
}
