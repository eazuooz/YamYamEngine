#pragma once
#include "yaEntity.h"
#include "imgui.h"

namespace gui
{
	class Widget : gui::Entity
	{
	public:
		enum class eState
		{
			Paused,
			Active,
			Dead,
		};

		Widget();
		~Widget();

		virtual void Update(); 
		virtual void FixedUpdate() = 0;
		virtual void Render();

		void AddWidget(Widget* child);

		void SetState(eState state) { mState = state; }
		eState GetState() { return mState; }
		void SetParent(Widget* parent) { mParent = parent; }
		Widget* GetParent() { return mParent; }

	private:
		eState mState;
		Widget* mParent;
		std::vector<Widget*> mChilds;
	};
}
