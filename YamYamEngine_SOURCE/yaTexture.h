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

		static void SetQuality(UINT quality) { Quality = quality; }
		static UINT GetQuality() { return Quality; }
		static void ClearShaderResourceView(UINT slot);
		static void ClearUnorderedAccessView(UINT slot);

		bool Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag);
		bool Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);
		bool CreateView(UINT bindFlag);
		bool CreateSRV();
		bool CreateUAV();
		bool CreateRTV();
		bool CreateDSV();

		void Reset();

		HRESULT Load(const std::wstring& path) override;
		void BindShaderResource(eShaderStage stage, UINT slot);
		void BindUnorderedAccessViews(UINT slot);
		

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    GetRTV() { return  mRTV; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>    GetDSV() { return  mDSV; }
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  GetSRV() { return  mSRV; }
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GetUAV() { return  mUAV; }
		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture() { return mTexture; }

		UINT GetHeight() { return mDesc.Height; }
		UINT GetWidth() { return mDesc.Width; }

	private:
		static UINT Quality;
		
		ScratchImage mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      mRTV;  
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      mDSV;  
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   mUAV;  
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;  
		D3D11_TEXTURE2D_DESC mDesc;
	};
}
