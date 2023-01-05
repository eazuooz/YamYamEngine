#pragma once
#include "yaComponent.h"


namespace ya
{
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		//virtual void Render();

	private:

	};
}
