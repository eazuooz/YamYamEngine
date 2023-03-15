#pragma once
#include "yaResource.h"
#include "..\External\Include\\DirectXTex\DirectXTex.h"
#include "yaGraphicsDevice_DX11.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\DirectXTex\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\External\\Library\\DirectXTex\\Release\\DirectXTex.lib")
#endif

namespace ya::graphics
{
	class Texture : public Resource
	{
	public:
		Texture();
		~Texture();

		static void Clear(UINT slot);

		bool Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag);
		bool Create();
		HRESULT Load(const std::wstring& path) override;
		void BindShaderResource(eShaderStage stage, UINT startSlot);
		void BindUnorderedAccessViews(UINT startSlot);
		void ClearUnorderedAccessViews(UINT startSlot);

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    GetRTV() { return  mRTV; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>    GetDSV() { return  mDSV; }
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  GetSRV() { return  mSRV; }
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GetUAV() { return  mUAV; }
		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture() { return mTexture; }
		void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture) { mTexture = texture; }

		UINT GetHeight() { return mDesc.Height; }
		UINT GetWidth() { return mDesc.Width; }

	private:
		ScratchImage mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      mRTV;  
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      mDSV;  
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   mUAV;  
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;  
		D3D11_TEXTURE2D_DESC mDesc;
	};
}
