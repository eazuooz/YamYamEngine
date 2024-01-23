#include "yaMaterial.h"
#include "yaShader.h"
#include "yaConstantBuffer.h"
#include "yaGraphicsDevice_DX11.h"

namespace ya::graphics
{
	Material::Material()
		: Resource(eResourceType::Material)
		, mShader(nullptr)
		, mMode(eRenderingMode::Opaque)
		, mTexture{}
	{
		mAttribute.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		mAttribute.ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);;
		mAttribute.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
		mAttribute.shininess = 1.0f;
	}

	Material::~Material()
	{
	}

	void Material::Bind()
	{
		for (size_t i = 0; i < (UINT)eTextureType::End; i++)
		{
			if (mTexture[i] == nullptr)
				continue;
			
			mTexture[i]->BindShaderResource(eShaderStage::VS, i);
			mTexture[i]->BindShaderResource(eShaderStage::HS, i);
			mTexture[i]->BindShaderResource(eShaderStage::DS, i);
			mTexture[i]->BindShaderResource(eShaderStage::GS, i);
			mTexture[i]->BindShaderResource(eShaderStage::PS, i);
			mTexture[i]->BindShaderResource(eShaderStage::CS, i);
		}

		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)graphics::eCBType::Material];

		if (mTexture[(UINT)eTextureType::Albedo]) { mAttribute.usedAlbedo = 1; }
		if (mTexture[(UINT)eTextureType::Specular]) { mAttribute.usedSpecular = 1; }
		if (mTexture[(UINT)eTextureType::Normal]) { mAttribute.usedNormal = 1; }

		pCB->SetData(&mAttribute);
		pCB->Bind(eShaderStage::ALL);

		mShader->Bind();
	}
	void Material::Clear()
	{
		for (size_t i = 0; i < (UINT)eTextureType::End; i++)
		{
			Texture::ClearShaderResourceView(i);
		}
	}

	std::shared_ptr<ImageFilter> Material::CreateImageFilter(const std::wstring& prefix, int width, int height
		, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
	{
		std::shared_ptr<ImageFilter> imageFilter = std::make_shared<ImageFilter>();
		imageFilter->Create(prefix, width, height, rtv, srv);
		mImageFilters.push_back(imageFilter);

		return imageFilter;
	}

	void Material::RenderImageFilters(UINT indexCount)
	{
		
		std::shared_ptr<ImageFilter> prevImageFilter = nullptr;
		for (std::shared_ptr<ImageFilter> imageFilter : mImageFilters)
		{
			if (prevImageFilter)
				imageFilter->SetPrevTargetShaderResource(prevImageFilter->GetRenderTarget());

			imageFilter->Render(indexCount);
			prevImageFilter = imageFilter;


		}


		GetDevice()->AdjustViewport();

		/*graphics::GetDevice()->OMSetRenderTargets(renderTargets.size()
			, renderTargets.data()
			, graphics::GetDevice()->GetDepthStencilTexture()->GetDSV().Get());*/
	}
}
