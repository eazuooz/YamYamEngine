#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiComponent.h"
#include "yaResource.h"

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
		ya::GameObject* GetTarget() { return mTargetGameObject; }
		void SetTargetResource(ya::Resource* resource) { mTargetResource = resource; }
		void SetTargetGameObject(ya::GameObject* gameObj) { mTargetGameObject = gameObj; }

	private:
		ya::GameObject* mTargetGameObject;
		ya::Resource* mTargetResource;
		std::vector<gui::Component*> mComponents;
	};
}
