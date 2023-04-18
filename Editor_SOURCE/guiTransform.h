#pragma once
#include "guiComponent.h"

namespace gui
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		virtual void Update() override;
		virtual void LateUpdate() override;
		//virtual void Render() override;

	private:
		ya::math::Vector3 mPosition;
		ya::math::Vector3 mRotation;
		ya::math::Vector3 mScale;
	};
}
