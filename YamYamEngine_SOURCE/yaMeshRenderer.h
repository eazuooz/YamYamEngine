#pragma once
#include "BaseRenderer.h"

namespace ya
{
	using namespace graphics;

	class MeshRenderer : public BaseRenderer
	{
	public:
		MeshRenderer();
		~MeshRenderer();
		
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
