#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiComponent.h"

namespace gui
{
	class Game : public Widget
	{
	public:
		Game();
		~Game();

		virtual void Update() override;
		virtual void LateUpdate() override;

	private:

	};
}


