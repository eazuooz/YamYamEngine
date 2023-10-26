#include "yaSkeletonShader.h"


namespace ya::graphics
{
	SkeletonShader::SkeletonShader()
		: rootMatrices(nullptr)
		, offsetMatrices(nullptr)
		, mGlobalMatrices(nullptr)
		, mAnimatorCB{}
	{
		mThreadGroupCountX = 32;
		mThreadGroupCountY = 32;
		mThreadGroupCountZ = 1;
	}
	SkeletonShader::~SkeletonShader()
	{
	}
	void SkeletonShader::Bind()
	{
		//StructuredBuffer<KeyFrame> keyFrames : register(t17);
		//StructuredBuffer<matrix> offsetMatrices : register(t18);
		//RWStructuredBuffer<matrix> globalMatrices : register(u2);

		//ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Animator];
		//cb->SetData(&mAnimatorCB);
		//cb->Bind(eShaderStage::VS);
		//cb->Bind(eShaderStage::PS);
		//cb->Bind(eShaderStage::CS);

		rootMatrices->BindSRV(eShaderStage::CS, 17);
		offsetMatrices->BindSRV(eShaderStage::CS, 18);
		mGlobalMatrices->BindUAV(2);

		mGroupX = (mAnimatorCB.boneCount / mThreadGroupCountX + 1);
		mGroupY = 1;
		mGroupZ = 1;
	}
	void SkeletonShader::Clear()
	{
		if (rootMatrices)
		{
			rootMatrices->Clear();
			rootMatrices = nullptr;
		}
		if (offsetMatrices)
		{
			offsetMatrices->Clear();
			offsetMatrices = nullptr;
		}
		if (mGlobalMatrices)
		{
			mGlobalMatrices->Clear();
			mGlobalMatrices = nullptr;
		}
	}
}

