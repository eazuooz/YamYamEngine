#pragma once
#include "yaGraphics.h"



namespace ya::graphics
{
	class GraphicsDevice
	{
	public:
		virtual ~GraphicsDevice() = default;

		virtual bool CreateSwapChain(const SwapChainDesc* desc, HWND hWnd, SwapChain* swapchain) = 0;
		virtual bool CreateBuffer(const GPUBufferDesc* desc, const void* initial_data, GPUBuffer* buffer) = 0;
		virtual bool CreateTexture(const TextureDesc* desc, const SubresourceData* initial_data, Texture* texture) = 0;
		virtual bool CreateSampler(const SamplerDesc* desc, Sampler* sampler) = 0;
		virtual bool CreateShader(ShaderStage stage, const void* shadercode, size_t shadercode_size, Shader* shader) = 0;

		virtual void Draw() = 0;

	};
}

