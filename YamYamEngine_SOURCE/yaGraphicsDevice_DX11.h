#pragma once
#include "yaGraphics.h"

namespace ya::graphics
{
	class GraphicsDevice_DX11 
	{
	public:
		GraphicsDevice_DX11();
		~GraphicsDevice_DX11();

		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC desc) ;
		bool CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* initial_data, ID3D11Buffer** buffer);
		bool CreateTexture(const D3D11_TEXTURE2D_DESC desc) ;
		bool CreateSampler();
		bool CreateShader(ShaderStage stage);


		void BindViewports(D3D11_VIEWPORT* viewPort);
		void BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void SetConstantBuffer(ShaderStage stage, CBTYPES type, ID3D11Buffer* buffer);
		
		
		void Draw();

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>			mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			mFrameBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			mDepthStencilBuffer;		
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>		mSamplers[(UINT)Filter::MAXIMUM_ANISOTROPIC];
		//D3D11_VIEWPORT mViewPort;
	};

	// This is a helper to get access to a global device instance
	//	- The engine uses this, but it is not necessary to use a single global device object
	//	- This is not a lifetime managing object, just a way to globally expose a reference to an object by pointer
	inline GraphicsDevice_DX11*& GetDevice()
	{
		static GraphicsDevice_DX11* device = nullptr;
		return device;
	}
}
