#include "yaParticleShader.h"
#include "yaRenderer.h"
#include "yaConstantBuffer.h"
#include "yaRenderer.h"
#include "yaTime.h"

namespace ya::graphics
{
	ParticleShader::ParticleShader()
		: ComputeShader(128, 1, 1)
		, mBuffer(nullptr)
	{
	}
	ParticleShader::~ParticleShader()
	{
	}
	void ParticleShader::Bind()
	{
		mBuffer->BindUAV(0);
		mSharedBuffer->BindUAV(1);

		mGroupX = mBuffer->GetStride() / mThreadGroupCountX + 1;
		mGroupY = 1;
		mGroupZ = 1;
	}
	void ParticleShader::Clear()
	{
		mBuffer->Clear();
		mSharedBuffer->Clear();
	}
}