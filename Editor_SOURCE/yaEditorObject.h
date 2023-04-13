#pragma once
#include "yaGameObject.h"

namespace gui
{
	class EditorObject : public ya::GameObject
	{
	public:
		EditorObject();
		~EditorObject();

		virtual void FixedUpdate() override;
	private:

	};
}
