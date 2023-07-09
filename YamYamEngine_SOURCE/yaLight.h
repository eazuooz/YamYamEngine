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

		virtual void LateUpdate() override;

		const LightAttribute& GetLightAttribute() { return mAttribute; }
		void SetColor(Vector4 color) { mAttribute.color = color; }

		void SetType(eLightType rype) { mAttribute.type = rype; }

		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }

		Vector4 GetColor() { return mAttribute.color; }
		eLightType GetType() { return mAttribute.type; }
		float GetRadius() { return mAttribute.radius; }
		float GetAngle() { return mAttribute.angle; }

	private:
		LightAttribute mAttribute;
	};
}
