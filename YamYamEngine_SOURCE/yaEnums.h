#pragma once

namespace ya::enums
{
	// Common blendmodes used across multiple systems
	enum BLENDMODE
	{
		BLENDMODE_OPAQUE,
		BLENDMODE_ALPHA,
		BLENDMODE_PREMULTIPLIED,
		BLENDMODE_ADDITIVE,
		BLENDMODE_MULTIPLY,
		BLENDMODE_COUNT
	};

	// Do not alter order or value because it is bound to lua manually!
	enum RENDERTYPE
	{
		RENDERTYPE_VOID = 0,
		RENDERTYPE_OPAQUE = 1 << 0,
		RENDERTYPE_TRANSPARENT = 1 << 1,
		RENDERTYPE_WATER = 1 << 2,
		RENDERTYPE_ALL = RENDERTYPE_OPAQUE | RENDERTYPE_TRANSPARENT | RENDERTYPE_WATER
	};

	enum RENDERPASS
	{
		RENDERPASS_MAIN,
		RENDERPASS_PREPASS,
		RENDERPASS_ENVMAPCAPTURE,
		RENDERPASS_SHADOW,
		RENDERPASS_SHADOWCUBE,
		RENDERPASS_VOXELIZE,
		RENDERPASS_COUNT
	};

	// There are two different kinds of stencil refs:
	//	ENGINE	: managed by the engine systems (STENCILREF enum values between 0-15)
	//	USER	: managed by the user (raw numbers between 0-15)
	enum STENCILREF_MASK
	{
		STENCILREF_MASK_ENGINE = 0x0F,
		STENCILREF_MASK_USER = 0xF0,
		STENCILREF_MASK_ALL = STENCILREF_MASK_ENGINE | STENCILREF_MASK_USER,
	};

	// engine stencil reference values. These can be in range of [0, 15].
	// Do not alter order or value because it is bound to lua manually!
	enum STENCILREF
	{
		STENCILREF_EMPTY = 0,
		STENCILREF_DEFAULT = 1,
		STENCILREF_CUSTOMSHADER = 2,
		STENCILREF_OUTLINE = 3,
		STENCILREF_CUSTOMSHADER_OUTLINE = 4,
		STENCILREF_LAST = 15
	};

	// constant buffers
	enum CBTYPES
	{

		CBTYPE_COUNT
	};

	// resource buffers (StructuredBuffer, Buffer, etc.)
	enum RBTYPES
	{

		RBTYPE_COUNT
	};

	// textures
	enum TEXTYPES
	{

		TEXTYPE_COUNT
	};

	// shaders
	enum SHADERTYPE
	{
		//vertex shader
		VSTYPE_DEFAULT_TRIANGLE,

		//pixel shader
		PSTYPE_DEFAULT_TRIANGLE,

		SHADERTYPE_COUNT,
	};

	// input layouts
	enum ILTYPES
	{
		ILTYPE_DEFAULT_TRIANGLE,

		ILTYPE_COUNT
	};
	// rasterizer states
	enum RSTYPES
	{

		RSTYPE_COUNT
	};
	// depth-stencil states
	enum DSSTYPES
	{

		DSSTYPE_COUNT
	};
	// blend states
	enum BSTYPES
	{

		BSTYPE_COUNT
	};

	enum SAMPLERTYPES
	{


		SAMPLER_COUNT,
	};

}
