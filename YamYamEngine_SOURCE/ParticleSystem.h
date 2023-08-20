#pragma once
#include "BaseRenderer.h"
#include "yaParticleShader.h"
#include "yaStructedBuffer.h"

namespace ya
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};

	class ParticleSystem : public BaseRenderer
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		void SetSimulationSpace(eSimulationSpace space) { mSimulationSpace = space; }

	private:
		graphics::StructedBuffer* mBuffer;
		graphics::StructedBuffer* mSharedBuffer;

		std::shared_ptr<graphics::ParticleShader> mCS;
		renderer::ParticleSystemCB mCBData;
		float	mTime;
		Vector4 mStartSize;
		Vector4 mStartColor;
		float   mStartLifeTime;
		float	mStartSpeed;
		float	mFrequency;
		UINT    mMaxParticles;
		eSimulationSpace mSimulationSpace;
		float	mRadius;
	};
}
