#pragma once
#include "yaEnums.h"
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaMath.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaConstantBuffer.h"
#include "yaCamera.h"
#include "yaStructedBuffer.h"
#include "yaTexture.h"
//#include "yaGameObject.h"


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
		Vector3 normal;
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

	CBUFFER(AnimatorCB, CBSLOT_ANIMATOR)
	{
		Vector2 leftTop;
		Vector2 size;
		Vector2 offset;
		Vector2 atlasSize;

		UINT type;
	};

	CBUFFER(LightCB, CBSLOT_LIGHT)
	{
		UINT numberOfLight;
	};
	
	CBUFFER(ParticleSystemCB, CBSLOT_PARTICLESYSTEM)
	{
		Vector4 ObjectWorldPos;
		// Particle System
		Vector4 startSize;
		Vector4 startColor;

		float   startLifeTime;
		float	startSpeed;
		UINT	maxParticles;
		UINT	simulationSpace;

		float	radius;
		float deltaTime;
		float elapsedTime;
	};

	CBUFFER(NoiseCB, CBSLOT_NOISETEXTURE)
	{
		Vector4 size;
		float noiseTime;
	};

	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizeStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStateStates[];

	extern std::vector<Camera*> cameras[];
	extern Camera* mainCamera;
	extern std::vector<LightAttribute> lights;
	extern StructedBuffer* lightsBuffer;
	extern std::shared_ptr<Texture> postProcessTexture;

	extern std::vector<DebugMesh> debugMeshes;
	extern GameObject* inspectorGameObject;

	// Initialize the renderer
	void Initialize();
	void Render();
	void DebugRender();
	void Release();
	void LoadMesh();
	void LoadMaterial();

	// Camera
	void PushDebugMesh(DebugMesh& mesh);

	// Light
	void PushLightAttribute(LightAttribute& lightAttribute);
	void BindLights();
	void BindNoiseTexture();

	// Post Process
	void CopyRenderTarget();
}
