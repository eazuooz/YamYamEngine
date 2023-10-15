
#pragma once
#include "BaseRenderer.h"

namespace ya
{
	using namespace graphics;

	class SkinnedMeshRenderer : public BaseRenderer
	{
	public:
		SkinnedMeshRenderer();
		~SkinnedMeshRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}