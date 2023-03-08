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
		void SetDiffuse(Vector4 color) { mAttribute.diffuse = color; }
		void SetAmbient(Vector4 color) { mAttribute.ambient = color; }

		void SetType(eLightType rype) { mAttribute.type = rype; }

		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }

		Vector4 GetDiffuse() { return mAttribute.diffuse; }
		eLightType GetType() { return mAttribute.type; }
		float GetRadius() { return mAttribute.radius; }
		float GetAngle() { return mAttribute.angle; }

	private:
		LightAttribute mAttribute;
	};
}
