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
		eLightType GetType() { return mAttribute.type; }
		void SetType(eLightType rype) { mAttribute.type = rype; }
		Vector4 GetColor() { return mAttribute.color; }
		void SetColor(Vector4 color) { mAttribute.color = color; }
		float GetRadius() { return mAttribute.radius; }
		void SetRadius(float radius) { mAttribute.radius = radius; }
		float GetAngle() { return mAttribute.angle; }
		void SetAngle(float angle) { mAttribute.angle = angle; }

	private:
		LightAttribute mAttribute;
	};
}
