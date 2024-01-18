#pragma once
#include "CommonInclude.h"
#include "yaEnums.h"
#include "yaMath.h"

#include <memory>
#include <string>
#include <vector>
#include <wrl.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

#define CBSLOT_TRANSFORM		0
#define	CBSLOT_MATERIAL			1
#define	CBSLOT_GRID				2
#define CBSLOT_ANIMATOR			3
#define CBSLOT_LIGHT			4
#define CBSLOT_PARTICLESYSTEM	5
#define CBSLOT_NOISETEXTURE		6
#define CBSLOT_TIME				7

using namespace ya::math;
namespace ya::graphics
{
	enum class eShaderStage 
	{
		VS,		// Vertex Shader
		HS,		// Hull Shader
		DS,		// Domain Shader
		GS,		// Geometry Shader
		PS,		// Pixel Shader
		CS,		// Compute Shader
		ALL,
		NONE,
	};

	// sampler Type
	enum class eSamplerType
	{
		Anisotropic,
		Point,
		PostProcess,
		End,
	};

	// constant buffers
	enum class eCBType 
	{
		Transform, //b0
		Material,  //b1
		Camera, //b2
		Animator, //b3
		Light, //b4
		ParticleSystem, //b5
		Noise, //b6
		Time, //b7
		End,
	};

	// reasterize type
	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,

		End,
	};

	// Depth Stencil type
	enum class eDSType
	{
		Less,
		Greater,
		NoWrite,
		None,
		End,
	};

	// Blend state type
	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,


		End,
	};

	enum class eGPUParam 
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Matrix,
	};

	enum class eRenderingMode
	{
		Opaque,
		CutOut,
		Transparent,
		PostProcess,

		End,
	};

	enum class eTextureType
	{
		Albedo,
		Normal,
		Specular,
		T3,
		T4,
		T5,
		T6,
		T7,

		DiffuseCube,
		SpecularCube,

		Array2DT10,
		Array2DT11,
		SpriteAtlas = 12,

		End,
	};



	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		End,
	};

	enum class eViewType
	{
		SRV,
		UAV,
		End,
	};

	enum class eStructedBuffer
	{
		Lights = 14,
		Particles = 16,
		End,
	};

	struct Viewport
	{
		float top_left_x = 0;
		float top_left_y = 0;
		float width = 0;
		float height = 0;
		float min_depth = 0;
		float max_depth = 1;
	};

	union ClearValue
	{
		float color[4];
		struct ClearDepthStencil
		{
			float depth;
			uint32_t stencil;
		} depth_stencil;
	};

	struct GpuBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		D3D11_BUFFER_DESC desc;

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;
	};

	struct DebugMesh
	{
		enums::eColliderType type;
		math::Vector3 position;
		math::Vector3 scale;
		math::Vector3 rotation;
		float radius;
		float duration;
		float time;
	};

	struct LightAttribute
	{
		Vector4 color;
		Vector4 position;
		Vector4 direction;
		
		eLightType type;
		float radius;
		float angle;
		float power;
	};
	
	struct Particle
	{
		Vector4 position;
		Vector4 direction;

		float maxTime;
		float time;
		float speed;
		UINT active;
	};

	struct ParticleShared
	{
		UINT sharedActiveCount;
	};

}
