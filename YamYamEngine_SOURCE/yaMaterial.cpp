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
	{
	}

	Material::~Material()
	{
	}

	void Material::SetData(graphics::eGPUParam param, void* data)
	{
		//switch (param)
		//{
		//case ya::graphics::eGPUParam::Int:
		//	mCB.iNum = *static_cast<int*>(data);
		//	break;
		//case ya::graphics::eGPUParam::Float:
		//	mCB.fNum = *static_cast<float*>(data);
		//	break;
		//case ya::graphics::eGPUParam::Vector2:
		//	mCB.vector2 = *static_cast<Vector2*>(data);
		//	break;
		//case ya::graphics::eGPUParam::Vector3:
		//	mCB.vector3 = *static_cast<Vector3*>(data);
		//	break;
		//case ya::graphics::eGPUParam::Vector4:
		//	mCB.vector4 = *static_cast<Vector4*>(data);
		//	break;
		//case ya::graphics::eGPUParam::Matrix:
		//	mCB.matrix = *static_cast<Matrix*>(data);
		//	break;
		//default:
		//	break;
		//}
	}

	void Material::Bind()
	{
		// Texture
		for (size_t i = 0; i < (UINT)eTextureType::End; i++)
		{
			if (mTexture[i] == nullptr)
				continue;
			
			mTexture[i]->BindShaderResource(eShaderStage::VS, 0);
			mTexture[i]->BindShaderResource(eShaderStage::HS, 0);
			mTexture[i]->BindShaderResource(eShaderStage::DS, 0);
			mTexture[i]->BindShaderResource(eShaderStage::GS, 0);
			mTexture[i]->BindShaderResource(eShaderStage::PS, 0);
			mTexture[i]->BindShaderResource(eShaderStage::CS, 0);
		}

		// Constant buffer
		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)graphics::eCBType::Material];
		//pCB->SetData(&mCB);
		pCB->Bind(eShaderStage::ALL);

		// Shader
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
