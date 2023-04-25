#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiComponent.h"

namespace gui
{
	class Hierarchy : public Widget
	{
	public:
		Hierarchy();
		~Hierarchy();

		virtual void Update() override;
		virtual void LateUpdate() override;

	private:

	};
}