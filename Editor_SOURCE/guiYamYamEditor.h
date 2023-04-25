#pragma once

#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiComponent.h"

namespace gui
{
	class YamYamEditor : public Widget
	{
	public:
		YamYamEditor();
		~YamYamEditor();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		

	private:
		bool mFullScreen;
		bool mPadding;
		ImGuiDockNodeFlags mDockspaceFlags;
	};
}