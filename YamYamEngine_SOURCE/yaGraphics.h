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
#define CBSLOT_ANIMATION		3

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
		NONE,
	};

	// sampler Type
	enum class eSamplerType
	{
		Anisotropic,
		Point,
		End,
	};

	// constant buffers
	enum class eCBType 
	{
		Transform, //b0
		Material,  //b1
		Grid, //b2
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

		End,
	};

	enum class eTextureSlot
	{
		t0,
		//t1,

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
}
