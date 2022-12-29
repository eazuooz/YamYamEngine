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
		bool CreateSampler(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
		
		//bool CreateShader(const graphics::ShaderStage stage, const std::wstring& file, const std::string& funcName);
		bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements
			, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout);
		bool CompileFromFile(const std::wstring& fileName, const std::string& funcName, const std::string& version, ID3DBlob** ppCode);
		bool CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11VertexShader** ppVertexShader);
		bool CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11PixelShader** ppVertexShader);

		void BindInputLayout(ID3D11InputLayout* pInputLayout);
		void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);
		void BindSamplers(eShaderStage stage, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);
		void BindsSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);
		void BindVertexBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT stride, const UINT offset = 0);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, UINT Offset = 0);

		void BindVertexShader(ID3D11VertexShader* pVertexShader);
		void BindPixelShader(ID3D11PixelShader* pPixelShader);

		void BindViewports(D3D11_VIEWPORT* viewPort);
		void BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void SetConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
		void SetShaderResource(eShaderStage stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV);

		void Clear();
		void AdjustViewport();
		void Draw(UINT VertexCount, UINT StartVertexLocation);
		void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
		void Present();
		void Render();

		ID3D11Device* GetID3D11Device() { return mDevice.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>			mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			mFrameBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			mDepthStencilBuffer;		
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			mSwapChain;
		//Microsoft::WRL::ComPtr<ID3D11SamplerState>		mSamplers[(UINT)Filter::MAXIMUM_ANISOTROPIC];
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
