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

		virtual void Update(); 
		virtual void LateUpdate() = 0;
		virtual void Render();
		virtual void Close() {}

		void AddWidget(Widget* child);
		void WindowFocus(bool enable);

		void SetState(eState state) { mState = state; }
		eState GetState() { return mState; }
		void SetParent(Widget* parent) { mParent = parent; }
		Widget* GetParent() { return mParent; }
		void SetSize(ImVec2 size) { mSize = size; }
		ImVec2 GetSize() { return mSize; }
		void SetFlag(ImGuiWindowFlags flag) { mFlag = flag; }

	private:
		void renderParent();
		void renderChild();

	private:
		eState mState;
		Widget* mParent;
		std::vector<Widget*> mChilds;

		ImGuiWindowFlags mFlag;
		ImVec2 mSize;
	};
}
