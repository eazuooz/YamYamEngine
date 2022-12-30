#include "yaTransform.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaRenderer.h"

namespace ya
{

	Transform::Transform()
		: Component(eComponentType::Transform)
	{

	}

	Transform::~Transform()
	{
	}

	void Transform::Initialize()
	{
	}

	void Transform::Update()
	{
	}

	void Transform::FixedUpdate()
	{
	}

	void Transform::Render()
	{
	}

	void Transform::SetConstantBuffer()
	{
		mPosition = Vector3(0.2f, 0.2f, 0.0f);

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)graphics::eCBType::Transform];
		cb->Bind(&mPosition);
		cb->SetPipline(graphics::eShaderStage::VS);
	}

}