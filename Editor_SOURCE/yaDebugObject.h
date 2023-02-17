#pragma once
#include "yaGameObject.h"


namespace ya
{
	class DebugObject : public GameObject
	{
	public:
		DebugObject();
		~DebugObject();

		virtual void FixedUpdate() override;
	};
}
