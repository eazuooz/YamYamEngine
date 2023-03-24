#pragma once
#include "BaseRenderer.h"
#include "yaParticleShader.h"

namespace ya
{
	class ParticleSystem : public BaseRenderer
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;


	private:
		class StructedBuffer* mBuffer;
		std::shared_ptr<ParticleShader> mCS;

		UINT    mCount;
		Vector4 mStartSize;
		Vector4 mEndSize;
		Vector4 mStartColor;
		Vector4 mEndColor;
		float   mMinLifeTime;
		float   mMaxLifeTime;
		float mFrequency;

	};
}
