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
		HRESULT Load(const std::wstring& path) override;
		void BindShader(eShaderStage stage, UINT startSlot);

		UINT GetHeight() { return mDesc.Height; }
		UINT GetWidth() { return mDesc.Width; }

	private:
		ScratchImage mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		D3D11_TEXTURE2D_DESC mDesc;
	};
}
