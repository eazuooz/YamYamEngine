#include "yaImageFilter.h"
#include "yaResources.h"


namespace ya
{
	ImageFilter::ImageFilter()
		: mViewPort{}
	{

	}

	ImageFilter::~ImageFilter()
	{

	}

	void ImageFilter::Create(const std::wstring& prefix, int width, int height
	, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
	{
		mRenderTarget = std::make_shared<graphics::Texture>();

		mRenderTarget->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS);

		mDepthStencil = std::make_shared<graphics::Texture>();
		mDepthStencil->Create(width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);

		graphics::GetDevice()->ClearRenderTargetView(mRenderTarget->GetRTV().Get());
		graphics::GetDevice()->ClearDepthStencilView(mDepthStencil->GetDSV().Get());

		if (rtv != nullptr)
			mRenderTarget->SetRTV(rtv);
		if (srv != nullptr)
			mRenderTarget->SetSRV(srv);

		mViewPort.TopLeftX = 0.0f;
		mViewPort.TopLeftY = 0.0f;
		mViewPort.Width = (FLOAT)(width);
		mViewPort.Height = (FLOAT)(height);
		mViewPort.MinDepth = 0.0f;
		mViewPort.MaxDepth = 1.0f;

		mShader = Resources::Find<graphics::Shader>(prefix);
	}

	void ImageFilter::Render(UINT indexCount)
	{
		mCB.dx = 1.0f / mViewPort.Width;
		mCB.dy = 1.0f / mViewPort.Height;

		graphics::ConstantBuffer* cb = renderer::constantBuffers[(UINT)graphics::eCBType::ImageFilter];
		cb->SetData(&mCB);
		cb->Bind(graphics::eShaderStage::PS);

		//���� �̹��������� Ÿ���� 60���������Ϳ� ���
		ID3D11ShaderResourceView* srv = nullptr;
		graphics::GetDevice()->BindShaderResource(graphics::eShaderStage::PS, 60, &srv);

		graphics::GetDevice()->BindShaderResource(graphics::eShaderStage::PS, 60
			, mPrevTargetSRV.GetAddressOf());

		//����Ÿ�� ����
		std::vector<ID3D11RenderTargetView*> renderTargets = {};
		renderTargets.push_back(mRenderTarget->GetRTV().Get());

		graphics::GetDevice()->OMSetRenderTargets(renderTargets.size()
			, renderTargets.data(), mDepthStencil->GetDSV().Get());

		// ����Ʈ ���� ������Ѵ�.
		graphics::GetDevice()->BindViewports(&mViewPort);

		mShader->Bind();
		graphics::GetDevice()->DrawIndexed(indexCount, 0, 0);

		graphics::GetDevice()->OMSetRenderTargets(0, nullptr, nullptr);
		graphics::GetDevice()->BindShaderResource(graphics::eShaderStage::PS, 60, &srv);
		graphics::GetDevice()->BindShaderResource(graphics::eShaderStage::PS, 60
			, mRenderTarget->GetSRV().GetAddressOf());
	}
}

