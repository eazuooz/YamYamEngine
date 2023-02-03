#include "yaTransform.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaRenderer.h"
#include "yaCamera.h"

namespace ya
{

	Transform::Transform()
		: Component(eComponentType::Transform)
		, mScale(Vector3::One)
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
		mWorld = Matrix::Identity;

		Matrix scale;
		scale = Matrix::CreateScale(mScale);

		Matrix rotation;
		rotation = Matrix::CreateRotationX(mRotation.x);
		rotation *= Matrix::CreateRotationY(mRotation.y);
		rotation *= Matrix::CreateRotationZ(mRotation.z);

		Matrix position;
		position.Translation(mPosition);

		mWorld = scale * rotation * position;

		// 기저벡터 구하기
		mUp = Vector3::TransformNormal(Vector3::Up, rotation);
		mFoward = Vector3::TransformNormal(Vector3::Forward, rotation);
		mRight = Vector3::TransformNormal(Vector3::Right, rotation);
	}

	void Transform::Render()
	{
	}

	void Transform::Translate(Vector3 translation)
	{

	}

	void Transform::SetConstantBuffer()
	{
		renderer::TransformCB trCB = {};
		trCB.mWorld = mWorld;
		trCB.mView = Camera::GetViewMatrix();
		trCB.mProjection = Camera::GetProjectionMatrix();

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)graphics::eCBType::Transform];
		cb->Bind(&trCB);
		cb->SetPipline(graphics::eShaderStage::VS);
	}

}