#include "guiWidget.h"


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
			//���� ������ �߾ӿ��� ��������
			//ImVec2 vCenter = ImGui::GetMainViewport()->GetCenter();
			//ImGui::SetNextWindowPos(vCenter, ImGuiCond_::ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			//ImGui::SetNextWindowSize(mSize, ImGuiCond_::ImGuiCond_Appearing);

			renderParent();
		}
		else
		{
			renderChild();
		}
	}

	void Widget::AddWidget(Widget* child)
	{
		mChilds.push_back(child);
		child->SetParent(this);
	}

	void Widget::WindowFocus(bool enable)
	{
		if (enable == true)
			ImGui::SetWindowFocus(GetName().c_str());
		else
			ImGui::SetWindowFocus(nullptr);
	}

	void Widget::renderParent()
	{
		bool Active = (bool)GetState();
		ImGui::Begin(GetName().c_str(), &Active);
		LateUpdate();
		for (Widget* child : mChilds)
		{
			child->Render();
		}
		ImGui::End();

		if (Active == false)
		{
			Close();
		}
	}

	void Widget::renderChild()
	{
		ImGui::BeginChild(GetName().c_str(), mSize);
		LateUpdate();
		for (Widget* child : mChilds)
		{
			child->Render();
		}
		ImGui::EndChild();
	}
}