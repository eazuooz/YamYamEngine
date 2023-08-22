#pragma once
#include "BaseRenderer.h"
#include "yaParticleShader.h"
//#include "yaRenderer.h"

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

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetSimulationSpace(const eSimulationSpace space) { mSimulationSpace = space; }

	private:
		StructedBuffer* mBuffer;
		StructedBuffer* mSharedBuffer;

		std::shared_ptr<ParticleShader> mCS;
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
