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

		virtual void LateUpdate() override;

		void SetTarget(ya::GameObject* gameObj) { mTarget = gameObj; }
		ya::GameObject* GetTarget() { return mTarget; }

	private:
		ya::enums::eComponentType mType;
		ya::GameObject* mTarget;
	};
}
