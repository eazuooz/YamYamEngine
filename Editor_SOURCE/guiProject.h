#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiComponent.h"
#include "guiTreeWidget.h"

namespace gui
{
	class Project : public Widget
	{
	public:
		Project();
		~Project();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void ResetContent();

	private:
		TreeWidget* mTreeWidget;
	};
}