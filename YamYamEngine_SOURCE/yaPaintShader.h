#pragma once
#include "yaComputeShader.h"
#include "yaTexture.h"

namespace ya::graphics
{
	class PaintShader: public ComputeShader
	{
	public:
		virtual void Binds() override;
		virtual void Clear() override;

		void SetTexture(std::shared_ptr<Texture> target) { mTarget = target; }

	private:
		std::shared_ptr<Texture> mTarget;
	};
}
