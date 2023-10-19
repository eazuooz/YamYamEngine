#pragma once
#include "yaComputeShader.h"
#include "yaStructedBuffer.h"

namespace ya::graphics
{
	class SkeletonShader : public ComputeShader
	{
	public:
		SkeletonShader();
		~SkeletonShader();

		virtual void Bind() override;
		virtual void Clear() override;

	private:
	};
}
