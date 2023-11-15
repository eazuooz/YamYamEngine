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
}
