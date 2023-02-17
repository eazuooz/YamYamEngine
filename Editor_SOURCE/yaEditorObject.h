#pragma once
#include "yaGameObject.h"

namespace ya
{
	class EditorObject : public GameObject
	{
	public:
		EditorObject();
		~EditorObject();

		virtual void FixedUpdate() override;
	private:

	};
}
