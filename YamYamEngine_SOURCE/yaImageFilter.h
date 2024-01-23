#pragma once
#include "yaGraphicsDevice_DX11.h"
#include "yaShader.h"
#include "yaTexture.h"

namespace ya
{
	class ImageFilter
	{
	public:
		ImageFilter();
		~ImageFilter();

        void Create(const std::wstring& prefix, int width, int height
         ,Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
		void Render(UINT indexCount);

        void SetRenderTarget(std::shared_ptr<graphics::Texture> resource)
        {
            mRenderTarget = resource;
        }
        void SetPrevTargetShaderResource(std::shared_ptr<graphics::Texture> resource)
        {
            mPrevTargetSRV = resource->GetSRV();
        }
        std::shared_ptr<graphics::Texture> GetRenderTarget() { return mRenderTarget; }


	private:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mPrevTargetSRV;
        std::shared_ptr<graphics::Texture> mRenderTarget;
        std::shared_ptr<graphics::Texture> mDepthStencil;
        D3D11_VIEWPORT mViewPort;
        //std::vector<> mRenderTargets;
        std::shared_ptr<graphics::Shader> mShader;
	};
}
