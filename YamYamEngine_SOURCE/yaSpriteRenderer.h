#pragma once
#include "BaseRenderer.h"

namespace ya
{
	using namespace graphics;

	class SpriteRenderer : public BaseRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
		void InitializeResource();
	};
}