#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"

namespace gui
{
	class Component : public Widget
	{
	public:
		Component(ya::enums::eComponentType type);
		~Component();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		ya::GameObject* GetTarget() { return mTarget; }
		void SetTarget(ya::GameObject* gameObj) { mTarget = gameObj; }

	private:
		ya::enums::eComponentType mType;
		ya::GameObject* mTarget;
	};
}
