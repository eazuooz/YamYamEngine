#include "yaParticleShader.h"
#include "yaRenderer.h"
#include "yaConstantBuffer.h"
#include "yaRenderer.h"
#include "yaTime.h"

namespace ya::graphics
{
	ParticleShader::ParticleShader()
		: ComputeShader(128, 1, 1)
		, mParticleBuffer(nullptr)
	{
	}
	ParticleShader::~ParticleShader()
	{
	}
	void ParticleShader::Binds()
	{
		mParticleBuffer->BindUAV(0);

		mGroupX = mParticleBuffer->GetStride() / mThreadGroupCountX + 1;
		mGroupY = 1;
		mGroupZ = 1;
	}
	void ParticleShader::Clear()
	{
		mParticleBuffer->Clear();
	}
	void ParticleShader::SetParticleBuffer(StructedBuffer* buffer)
	{
		mParticleBuffer = buffer;
		renderer::ParticleSystemCB info = {};
		info.elementCount = mParticleBuffer->GetStride();
		info.deltaTime = Time::DeltaTime();

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::ParticleSystem];
		cb->SetData(&info);
		cb->Bind(eShaderStage::CS);
	}
}