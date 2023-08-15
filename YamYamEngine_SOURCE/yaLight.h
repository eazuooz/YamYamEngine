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

		void LateUpdate() override;

		const LightAttribute& GetLightAttribute() const { return mAttribute; }
		eLightType GetType() const { return mAttribute.type; }
		void SetType(eLightType type) { mAttribute.type = type; }
		Vector4 GetColor() const { return mAttribute.color; }
		void SetColor(Vector4 color) { mAttribute.color = color; }
		float GetRadius() const { return mAttribute.radius; }
		void SetRadius(float radius) { mAttribute.radius = radius; }
		float GetAngle() const { return mAttribute.angle; }
		void SetAngle(float angle) { mAttribute.angle = angle; }

	private:
		LightAttribute mAttribute;
	};
}
