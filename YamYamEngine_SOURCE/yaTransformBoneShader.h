#pragma once
#include "yaComputeShader.h"
#include "yaStructedBuffer.h"
#include "yaRenderer.h"


namespace ya::graphics
{
	class TransformBoneShader : public ComputeShader
	{
	public:
		TransformBoneShader();
		~TransformBoneShader();

		virtual void Bind() override;
		virtual void Clear() override;

		void SetRootMatrices(StructedBuffer* buffer) { rootMatrices = buffer; }
		void SetOffsetMatrices(StructedBuffer* buffer) { offsetMatrices = buffer; }
		void SetGlobalMatrices(StructedBuffer* buffer) { mGlobalMatrices = buffer; }
		void SetAnimatorCB(renderer::AnimatorCB data) { mAnimatorCB = data; };

	private:
		StructedBuffer* rootMatrices;
		StructedBuffer* offsetMatrices;
		StructedBuffer* mGlobalMatrices;

		renderer::AnimatorCB mAnimatorCB;
	};
}
