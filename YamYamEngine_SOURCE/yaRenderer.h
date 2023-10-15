#pragma once
#include "yaEnums.h"
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaMath.h"
#include "yaConstantBuffer.h"
#include "yaCamera.h"
#include "yaStructedBuffer.h"
#include "yaTexture.h"

namespace ya::renderer
{
	using namespace graphics;
	using namespace enums;
	using namespace math;

	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
		Vector2 uv;
		Vector3 normal;
		Vector3 biNormal;
		Vector3 tangent;

		//boneWeight
		int boneIndex0;
		int boneIndex1;
		int boneIndex2;
		int boneIndex3;

		double weight0;
		double weight1;
		double weight2;
		double weight3;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		Vector4 diffuse;
		Vector4 ambient;
		Vector4 specular;

		float shininess;
		int usedAlbedo;
		float dummy2;
		float dummy3;
	};

	CBUFFER(CameraCB, CBSLOT_GRID)
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
		UINT lightCount;
	};
	
	CBUFFER(ParticleSystemCB, CBSLOT_PARTICLESYSTEM)
	{
		Vector4 position;
		Vector4 startSize;
		Vector4 startColor;

		float   startLifeTime;
		float	startSpeed;
		UINT	maxParticles;
		UINT	simulationSpace;

		float	radius;
		float	deltaTime;
		float	elapsedTime;
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
	extern std::shared_ptr<Texture> postProcessing;
	extern std::shared_ptr<Texture> renderTarget;

	extern std::vector<DebugMesh> debugMeshes;
	extern GameObject* inspectorGameObject;

	// Initialize the renderer
	void Initialize();
	void Render();
	void DebugRender();
	void Release();

	// Camera
	void PushDebugMesh(DebugMesh& mesh);

	// Light
	void PushLightAttribute(LightAttribute& lightAttribute);
	void BindLights();
	void BindNoiseTexture();

	// Post Process
	void CopyRenderTarget();
}
