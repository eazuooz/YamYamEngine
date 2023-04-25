#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiComponent.h"

namespace gui
{
	class Project : public Widget
	{
	public:
		Project();
		~Project();

		virtual void Update() override;
		virtual void LateUpdate() override;

	private:

	};
}