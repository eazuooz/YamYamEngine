#pragma once
#include "yaEntity.h"
#include "imgui.h"

namespace gui
{


	class Widget : public gui::Entity
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

		virtual void FixedUpdate();
		virtual void Update(); 
		virtual void LateUpdate() = 0;
		virtual void Close() {}
		
		void Render();

		void AddWidget(Widget* child);
		void WindowFocus(bool enable);

		void SetState(eState state) { mState = state; }
		eState GetState() { return mState; }
		void SetParent(Widget* parent) { mParent = parent; }
		Widget* GetParent() { return mParent; }
		void SetSize(ImVec2 size) { mSize = size; }
		ImVec2 GetSize() { return mSize; }

	private:
		void renderParent();
		void renderChild();

	protected:
		ImGuiWindowFlags mFlag;

	private:
		eState mState;
		Widget* mParent;
		std::vector<Widget*> mChilds;

		ImVec2 mSize;
	};
}
