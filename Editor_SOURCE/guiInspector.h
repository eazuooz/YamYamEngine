#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiComponent.h"

namespace gui
{
	class Inspector : public Widget
	{
	public:
		Inspector();
		~Inspector();

		virtual void Update() override;
		virtual void LateUpdate() override;

		void InitializeTarget(ya::GameObject* gameObj);
		ya::GameObject* GetTarget() { return mTarget; }

	private:
		ya::GameObject* mTarget;
		std::vector<gui::Component*> mComponents;
	};
}
