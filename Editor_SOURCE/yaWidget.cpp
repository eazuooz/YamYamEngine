#include "yaWidget.h"


namespace gui
{
	Widget::Widget()
		: mState(eState::Active)
		, mParent(nullptr)
	{
	}

	Widget::~Widget()
	{

	}

	void Widget::Update()
	{
		if (mState == eState::Active)
		{
			for (Widget* widget : mChilds)
			{
				widget->Update();
			}
		}
	}

	void Widget::Render()
	{
		if (mState != eState::Active)
			return;

		if (mParent == nullptr)
		{
			bool open = (bool)mState;
			ImGui::Begin(GetName().c_str(), &open);

			FixedUpdate();
			for (Widget* child : mChilds)
			{
				child->Render();
			}

			ImGui::End();
		}
		else
		{
			ImGui::BeginChild(GetName().c_str());

			FixedUpdate();
			for (Widget* child : mChilds)
			{
				child->Render();
			}

			ImGui::EndChild();
		}
	}

	void Widget::AddWidget(Widget* child)
	{
		mChilds.push_back(child);
		child->SetParent(this);
	}
}