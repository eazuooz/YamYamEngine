#include "yaMaterial.h"
#include "yaShader.h"
#include "yaConstantBuffer.h"

namespace ya
{
	Material::Material()
		: Resource(eResourceType::Material)
		, mShader(nullptr)
	{
	}

	Material::~Material()
	{
	}

	void Material::SetData(graphics::eGPUParam param, void* data)
	{
		switch (param)
		{
		case ya::graphics::eGPUParam::Int:
			mCB.iNum = *static_cast<int*>(data);
			break;
		case ya::graphics::eGPUParam::Float:
			mCB.fNum = *static_cast<float*>(data);
			break;
		case ya::graphics::eGPUParam::Vector2:
			mCB.vector2 = *static_cast<Vector2*>(data);
			break;
		case ya::graphics::eGPUParam::Vector3:
			mCB.vector3 = *static_cast<Vector3*>(data);
			break;
		case ya::graphics::eGPUParam::Vector4:
			mCB.vector4 = *static_cast<Vector4*>(data);
			break;
		case ya::graphics::eGPUParam::Matrix:
			mCB.matrix = *static_cast<Matrix*>(data);
			break;
		default:
			break;
		}
	}

	void Material::Bind()
	{
		// Texture
		mTexture->BindShader(eShaderStage::PS, 0);

		//std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"TriangleTexture", L"..\\Resources\\Triangle.png");
		//texture->BindShader(eShaderStage::PS, 0);

		// Constant buffer
		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)graphics::eCBType::Material];
		pCB->Bind(&mCB);
		pCB->SetPipline(eShaderStage::VS);
		pCB->SetPipline(eShaderStage::PS);

		// Shader
		mShader->Bind();
	}
}
