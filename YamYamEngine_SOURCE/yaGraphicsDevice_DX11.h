#pragma once
#include "yaGraphicsDevice.h"


#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace ya::graphics
{
	class GraphicsDevice_DX11 final : public GraphicsDevice
	{
	public:
		GraphicsDevice_DX11(ValidationMode validationMode = ValidationMode::Disabled);
		~GraphicsDevice_DX11();

		
		virtual bool CreateSwapChain(const SwapChainDesc* desc, HWND hWnd, SwapChain* swapchain) override;
		virtual bool CreateBuffer(const GPUBufferDesc* desc, const void* initial_data, GPUBuffer* buffer) override;
		virtual bool CreateTexture(const TextureDesc* desc, const SubresourceData* initial_data, Texture* texture) override;
		virtual bool CreateSampler(const SamplerDesc* desc, Sampler* sampler) override;
		virtual bool CreateShader(ShaderStage stage, const void* shadercode, size_t shadercode_size, Shader* shader) override;

		void Draw() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>			mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			mRenderTarget;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			mDepthStencilBuffer;		
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;

		//IDXGISwapChain*			mSwapChain;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>		mSamplers[(UINT)Filter::MAXIMUM_ANISOTROPIC];
	};
}
