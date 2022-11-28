#pragma once
#include "CommonInclude.h"

#include <memory>
#include <string>
#include <vector>
#include <wrl.h>

namespace ya::graphics
{
	// enum
	enum class ValidationMode
	{
		Disabled,	// No validation is enabled
		Enabled,	// CPU command validation
		GPU			// CPU and GPU-based validation
	};

	enum class ShaderStage
	{
		MS,		// Mesh Shader
		AS,		// Amplification Shader
		VS,		// Vertex Shader
		HS,		// Hull Shader
		DS,		// Domain Shader
		GS,		// Geometry Shader
		PS,		// Pixel Shader
		CS,		// Compute Shader
		LIB,	// Shader Library
		Count,
	};
	enum class ShaderFormat
	{
		NONE,	// Not used
		HLSL5,	// DXBC
		HLSL6,	// DXIL
		SPIRV,	// SPIR-V
	};
	enum class ShaderModel
	{
		SM_5_0,
		SM_6_0,
		SM_6_1,
		SM_6_2,
		SM_6_3,
		SM_6_4,
		SM_6_5,
		SM_6_6,
		SM_6_7,
	};
	enum class PrimitiveTopology
	{
		UNDEFINED,
		TRIANGLELIST,
		TRIANGLESTRIP,
		POINTLIST,
		LINELIST,
		LINESTRIP,
		PATCHLIST,
	};
	enum class ComparisonFunc
	{
		NEVER,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_EQUAL,
		ALWAYS,
	};
	enum class DepthWriteMask
	{
		ZERO,	// Disables depth write
		ALL,	// Enables depth write
	};
	enum class StencilOp
	{
		KEEP,
		ZERO,
		REPLACE,
		INCR_SAT,
		DECR_SAT,
		INVERT,
		INCR,
		DECR,
	};
	enum class Blend
	{
		ZERO,
		ONE,
		SRC_COLOR,
		INV_SRC_COLOR,
		SRC_ALPHA,
		INV_SRC_ALPHA,
		DEST_ALPHA,
		INV_DEST_ALPHA,
		DEST_COLOR,
		INV_DEST_COLOR,
		SRC_ALPHA_SAT,
		BLEND_FACTOR,
		INV_BLEND_FACTOR,
		SRC1_COLOR,
		INV_SRC1_COLOR,
		SRC1_ALPHA,
		INV_SRC1_ALPHA,
	};
	enum class BlendOp
	{
		ADD,
		SUBTRACT,
		REV_SUBTRACT,
		MIN,
		MAX,
	};
	enum class FillMode
	{
		WIREFRAME,
		SOLID,
	};
	enum class CullMode
	{
		NONE,
		FRONT,
		BACK,
	};
	enum class InputClassification
	{
		PER_VERTEX_DATA,
		PER_INSTANCE_DATA,
	};
	enum class Usage
	{
		DEFAULT = 0,
		IMMUTABLE = 1,
		DYNAMIC = 2,
		STAGING = 3
	};
	enum class TextureAddressMode
	{
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIRROR_ONCE,
	};
	enum class Filter
	{
		MIN_MAG_MIP_POINT,
		MIN_MAG_POINT_MIP_LINEAR,
		MIN_POINT_MAG_LINEAR_MIP_POINT,
		MIN_POINT_MAG_MIP_LINEAR,
		MIN_LINEAR_MAG_MIP_POINT,
		MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		MIN_MAG_LINEAR_MIP_POINT,
		MIN_MAG_MIP_LINEAR,
		ANISOTROPIC,
		COMPARISON_MIN_MAG_MIP_POINT,
		COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
		COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
		COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
		COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
		COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
		COMPARISON_MIN_MAG_MIP_LINEAR,
		COMPARISON_ANISOTROPIC,
		MINIMUM_MIN_MAG_MIP_POINT,
		MINIMUM_MIN_MAG_POINT_MIP_LINEAR,
		MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
		MINIMUM_MIN_POINT_MAG_MIP_LINEAR,
		MINIMUM_MIN_LINEAR_MAG_MIP_POINT,
		MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		MINIMUM_MIN_MAG_LINEAR_MIP_POINT,
		MINIMUM_MIN_MAG_MIP_LINEAR,
		MINIMUM_ANISOTROPIC,
		MAXIMUM_MIN_MAG_MIP_POINT,
		MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,
		MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
		MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
		MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
		MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
		MAXIMUM_MIN_MAG_MIP_LINEAR,
		MAXIMUM_ANISOTROPIC,
	};
	enum class SamplerBorderColor
	{
		TRANSPARENT_BLACK,
		OPAQUE_BLACK,
		OPAQUE_WHITE,
	};

	enum class Format
	{
		UNKNOWN = 0,
		R32G32B32A32_TYPELESS = 1,
		R32G32B32A32_FLOAT = 2,
		R32G32B32A32_UINT = 3,
		R32G32B32A32_SINT = 4,
		R32G32B32_TYPELESS = 5,
		R32G32B32_FLOAT = 6,
		R32G32B32_UINT = 7,
		R32G32B32_SINT = 8,
		R16G16B16A16_TYPELESS = 9,
		R16G16B16A16_FLOAT = 10,
		R16G16B16A16_UNORM = 11,
		R16G16B16A16_UINT = 12,
		R16G16B16A16_SNORM = 13,
		R16G16B16A16_SINT = 14,
		R32G32_TYPELESS = 15,
		R32G32_FLOAT = 16,
		R32G32_UINT = 17,
		R32G32_SINT = 18,
		R32G8X24_TYPELESS = 19,
		D32_FLOAT_S8X24_UINT = 20,
		R32_FLOAT_X8X24_TYPELESS = 21,
		X32_TYPELESS_G8X24_UINT = 22,
		R10G10B10A2_TYPELESS = 23,
		R10G10B10A2_UNORM = 24,
		R10G10B10A2_UINT = 25,
		R11G11B10_FLOAT = 26,
		R8G8B8A8_TYPELESS = 27,
		R8G8B8A8_UNORM = 28,
		R8G8B8A8_UNORM_SRGB = 29,
		R8G8B8A8_UINT = 30,
		R8G8B8A8_SNORM = 31,
		R8G8B8A8_SINT = 32,
		R16G16_TYPELESS = 33,
		R16G16_FLOAT = 34,
		R16G16_UNORM = 35,
		R16G16_UINT = 36,
		R16G16_SNORM = 37,
		R16G16_SINT = 38,
		R32_TYPELESS = 39,
		D32_FLOAT = 40,
		R32_FLOAT = 41,
		R32_UINT = 42,
		R32_SINT = 43,
		R24G8_TYPELESS = 44,
		D24_UNORM_S8_UINT = 45,
		R24_UNORM_X8_TYPELESS = 46,
		X24_TYPELESS_G8_UINT = 47,
		R8G8_TYPELESS = 48,
		R8G8_UNORM = 49,
		R8G8_UINT = 50,
		R8G8_SNORM = 51,
		R8G8_SINT = 52,
		R16_TYPELESS = 53,
		R16_FLOAT = 54,
		D16_UNORM = 55,
		R16_UNORM = 56,
		R16_UINT = 57,
		R16_SNORM = 58,
		R16_SINT = 59,
		R8_TYPELESS = 60,
		R8_UNORM = 61,
		R8_UINT = 62,
		R8_SNORM = 63,
		R8_SINT = 64,
		A8_UNORM = 65,
		R1_UNORM = 66,
		R9G9B9E5_SHAREDEXP = 67,
		R8G8_B8G8_UNORM = 68,
		G8R8_G8B8_UNORM = 69,
		BC1_TYPELESS = 70,
		BC1_UNORM = 71,
		BC1_UNORM_SRGB = 72,
		BC2_TYPELESS = 73,
		BC2_UNORM = 74,
		BC2_UNORM_SRGB = 75,
		BC3_TYPELESS = 76,
		BC3_UNORM = 77,
		BC3_UNORM_SRGB = 78,
		BC4_TYPELESS = 79,
		BC4_UNORM = 80,
		BC4_SNORM = 81,
		BC5_TYPELESS = 82,
		BC5_UNORM = 83,
		BC5_SNORM = 84,
		B5G6R5_UNORM = 85,
		B5G5R5A1_UNORM = 86,
		B8G8R8A8_UNORM = 87,
		B8G8R8X8_UNORM = 88,
		R10G10B10_XR_BIAS_A2_UNORM = 89,
		B8G8R8A8_TYPELESS = 90,
		B8G8R8A8_UNORM_SRGB = 91,
		B8G8R8X8_TYPELESS = 92,
		B8G8R8X8_UNORM_SRGB = 93,
		BC6H_TYPELESS = 94,
		BC6H_UF16 = 95,
		BC6H_SF16 = 96,
		BC7_TYPELESS = 97,
		BC7_UNORM = 98,
		BC7_UNORM_SRGB = 99,
		AYUV = 100,
		Y410 = 101,
		Y416 = 102,
		NV12 = 103,
		P010 = 104,
		P016 = 105,
		//420_OPAQUE = 106,
		YUY2 = 107,
		Y210 = 108,
		Y216 = 109,
		NV11 = 110,
		AI44 = 111,
		IA44 = 112,
		P8 = 113,
		A8P8 = 114,
		B4G4R4A4_UNORM = 115,

		P208 = 130,
		V208 = 131,
		V408 = 132,

		SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189,
		SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,

		FORCE_UINT = 0xffffffff
	};

	enum class GpuQueryType
	{
		TIMESTAMP,			// retrieve time point of gpu execution
		OCCLUSION,			// how many samples passed depth test?
		OCCLUSION_BINARY,	// depth test passed or not?
	};
	enum class IndexBufferFormat
	{
		UINT16,
		UINT32,
	};
	enum class SubresourceType
	{
		SRV, // shader resource view
		UAV, // unordered access view
		RTV, // render target view
		DSV, // depth stencil view
	};

	enum class ShadingRate
	{
		RATE_1X1,	// Default/full shading rate
		RATE_1X2,
		RATE_2X1,
		RATE_2X2,
		RATE_2X4,
		RATE_4X2,
		RATE_4X4,

		RATE_INVALID
	};

	enum class PredicationOp
	{
		EQUAL_ZERO,
		NOT_EQUAL_ZERO,
	};

	// Flags ////////////////////////////////////////////

	enum class ColorWrite
	{
		DISABLE = 0,
		ENABLE_RED = 1 << 0,
		ENABLE_GREEN = 1 << 1,
		ENABLE_BLUE = 1 << 2,
		ENABLE_ALPHA = 1 << 3,
		ENABLE_ALL = ~0,
	};

	enum class BindFlag
	{
		NONE = 0,
		VERTEX_BUFFER = 0x1L,
		INDEX_BUFFER = 0x2L,
		CONSTANT_BUFFER = 0x4L,
		SHADER_RESOURCE = 0x8L,
		STREAM_OUTPUT = 0x10L,
		RENDER_TARGET = 0x20L,
		DEPTH_STENCIL = 0x40L,
		UNORDERED_ACCESS = 0x80L,
		DECODER = 0x200L,
		VIDEO_ENCODER = 0x400L
	};

	enum class ResourceMiscFlag
	{
		NONE = 0,
		TEXTURECUBE = 1 << 0,
		INDIRECT_ARGS = 1 << 1,
		BUFFER_RAW = 1 << 2,
		BUFFER_STRUCTURED = 1 << 3,
		RAY_TRACING = 1 << 4,
		PREDICATION = 1 << 5,
		TRANSIENT_ATTACHMENT = 1 << 6,	// hint: used in renderpass, without needing to write content to memory (VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
	};

	enum class GraphicsDeviceCapability
	{
		NONE = 0,
		TESSELLATION = 1 << 0,
		CONSERVATIVE_RASTERIZATION = 1 << 1,
		RASTERIZER_ORDERED_VIEWS = 1 << 2,
		UAV_LOAD_FORMAT_COMMON = 1 << 3, // eg: R16G16B16A16_FLOAT, R8G8B8A8_UNORM and more common ones
		UAV_LOAD_FORMAT_R11G11B10_FLOAT = 1 << 4,
		RENDERTARGET_AND_VIEWPORT_ARRAYINDEX_WITHOUT_GS = 1 << 5,
		VARIABLE_RATE_SHADING = 1 << 6,
		VARIABLE_RATE_SHADING_TIER2 = 1 << 7,
		MESH_SHADER = 1 << 8,
		RAYTRACING = 1 << 9,
		PREDICATION = 1 << 10,
		SAMPLER_MINMAX = 1 << 11,
		DEPTH_BOUNDS_TEST = 1 << 12,
	};

	enum class ResourceState
	{
		// Common resource states:
		UNDEFINED = 0,						// invalid state (don't preserve contents)
		SHADER_RESOURCE = 1 << 0,			// shader resource, read only
		SHADER_RESOURCE_COMPUTE = 1 << 1,	// shader resource, read only, non-pixel shader
		UNORDERED_ACCESS = 1 << 2,			// shader resource, write enabled
		COPY_SRC = 1 << 3,					// copy from
		COPY_DST = 1 << 4,					// copy to

		// Texture specific resource states:
		RENDERTARGET = 1 << 5,				// render target, write enabled
		DEPTHSTENCIL = 1 << 6,				// depth stencil, write enabled
		DEPTHSTENCIL_READONLY = 1 << 7,		// depth stencil, read only
		SHADING_RATE_SOURCE = 1 << 8,		// shading rate control per tile

		// GPUBuffer specific resource states:
		VERTEX_BUFFER = 1 << 9,				// vertex buffer, read only
		INDEX_BUFFER = 1 << 10,				// index buffer, read only
		CONSTANT_BUFFER = 1 << 11,			// constant buffer, read only
		INDIRECT_ARGUMENT = 1 << 12,			// argument buffer to DrawIndirect() or DispatchIndirect()
		RAYTRACING_ACCELERATION_STRUCTURE = 1 << 13, // acceleration structure storage or scratch
		PREDICATION = 1 << 14				// storage for predication comparison value
	};

	enum class ColorSpace
	{
		SRGB,			// SDR color space (8 or 10 bits per channel)
		HDR10_ST2084,	// HDR10 color space (10 bits per channel)
		HDR_LINEAR,		// HDR color space (16 bits per channel)
	};

	// Descriptor structs:

	struct Viewport
	{
		float top_left_x = 0;
		float top_left_y = 0;
		float width = 0;
		float height = 0;
		float min_depth = 0;
		float max_depth = 1;
	};

	struct InputLayout
	{
		static const uint32_t APPEND_ALIGNED_ELEMENT = ~0u; // automatically figure out AlignedByteOffset depending on Format

		struct Element
		{
			std::string semantic_name;
			uint32_t semantic_index = 0;
			Format format = Format::UNKNOWN;
			uint32_t input_slot = 0;
			uint32_t aligned_byte_offset = APPEND_ALIGNED_ELEMENT;
			InputClassification input_slot_class = InputClassification::PER_VERTEX_DATA;
		};
		std::vector<Element> elements;
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

	//Desc

	struct SwapChainDesc
	{
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t buffer_count = 2;
		Format format = Format::R8G8B8A8_UNORM;
		bool fullscreen = false;
		bool vsync = true;
		float clear_color[4] = { 0,0,0,1 };
		bool allow_hdr = true;
	};

	struct GPUBufferDesc
	{
		uint64_t size = 0;
		Usage usage = Usage::DEFAULT;
		BindFlag bind_flags = BindFlag::NONE;
		ResourceMiscFlag misc_flags = ResourceMiscFlag::NONE;
		uint32_t stride = 0; // only needed for structured buffer types!
		Format format = Format::UNKNOWN; // only needed for typed buffer!
	};

	struct TextureDesc
	{
		enum class Type
		{
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_3D,
		} type = Type::TEXTURE_2D;
		uint32_t width = 1;
		uint32_t height = 1;
		uint32_t depth = 1;
		uint32_t array_size = 1;
		uint32_t mip_levels = 1;
		Format format = Format::UNKNOWN;
		uint32_t sample_count = 1;
		Usage usage = Usage::DEFAULT;
		BindFlag bind_flags = BindFlag::NONE;
		ResourceMiscFlag misc_flags = ResourceMiscFlag::NONE;
		ClearValue clear = {};
		//ResourceState layout = ResourceState::SHADER_RESOURCE;
	};

	struct SamplerDesc
	{
		Filter filter = Filter::MIN_MAG_MIP_POINT;
		TextureAddressMode address_u = TextureAddressMode::CLAMP;
		TextureAddressMode address_v = TextureAddressMode::CLAMP;
		TextureAddressMode address_w = TextureAddressMode::CLAMP;
		float mip_lod_bias = 0;
		uint32_t max_anisotropy = 0;
		ComparisonFunc comparison_func = ComparisonFunc::NEVER;
		SamplerBorderColor border_color = SamplerBorderColor::TRANSPARENT_BLACK;
		//float min_lod = 0;
		//float max_lod = std::numeric_limits<float>::max();
	};

	struct SubresourceData
	{
		const void* data_ptr = nullptr;	// pointer to the beginning of the subresource data (pointer to beginning of resource + subresource offset)
		uint32_t row_pitch = 0;			// bytes between two rows of a texture (2D and 3D textures)
		uint32_t slice_pitch = 0;		// bytes between two depth slices of a texture (3D textures only)
	};

	// Resource

	struct GraphicsDeviceChild
	{
		std::shared_ptr<void> internal_state;
		inline bool IsValid() const { return internal_state.get() != nullptr; }
	};

	struct Sampler : public GraphicsDeviceChild
	{
		SamplerDesc desc;

		const SamplerDesc& GetDesc() const { return desc; }
	};

	struct Shader : public GraphicsDeviceChild
	{
		ShaderStage stage = ShaderStage::Count;
	};

	struct GPUResource : public GraphicsDeviceChild
	{
		enum class Type
		{
			BUFFER,
			TEXTURE,
			RAYTRACING_ACCELERATION_STRUCTURE,
			UNKNOWN_TYPE,
		} type = Type::UNKNOWN_TYPE;
		constexpr bool IsTexture() const { return type == Type::TEXTURE; }
		constexpr bool IsBuffer() const { return type == Type::BUFFER; }
		constexpr bool IsAccelerationStructure() const { return type == Type::RAYTRACING_ACCELERATION_STRUCTURE; }

		// These are only valid if the resource was created with CPU access (USAGE::UPLOAD or USAGE::READBACK)
		void* mapped_data = nullptr;	// for buffers, it is a pointer to the buffer data; for textures, it is a pointer to texture data with linear tiling;
		size_t mapped_size = 0;			// for buffers, it is the full buffer size; for textures it is the full texture size including all subresources;
	};

	struct SwapChain : public GraphicsDeviceChild
	{
		SwapChainDesc desc;

		constexpr const SwapChainDesc& GetDesc() const { return desc; }
	};

	struct GPUBuffer : public GPUResource
	{
		GPUBufferDesc desc;

		constexpr const GPUBufferDesc& GetDesc() const { return desc; }
	};



	struct Texture : public GPUResource
	{
		TextureDesc	desc;

		// These are only valid if the texture was created with CPU access (USAGE::UPLOAD or USAGE::READBACK)
		const SubresourceData* mapped_subresources = nullptr;	// an array of subresource mappings in the following memory layout: slice0|mip0, slice0|mip1, slice0|mip2, ... sliceN|mipN
		size_t mapped_subresource_count = 0;					// the array size of mapped_subresources (number of slices * number of miplevels)

		constexpr const TextureDesc& GetDesc() const { return desc; }
	};
}
