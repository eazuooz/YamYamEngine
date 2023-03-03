#pragma once
#include "yaComponent.h"
#include "yaGraphics.h"

namespace ya
{
	using namespace graphics;

	class Light : public Component
	{
	public:
		Light();
		~Light();

		virtual void FixedUpdate() override;

		const LightAttribute& GetLightAttribute() { return mAttribute; }

	private:
		LightAttribute mAttribute;
	};
}
