#include "guiListWidget.h"


namespace gui
{
	ListWidget::ListWidget()
		: Widget()
	{
		SetName("ListWidget");
		SetState(eState::Paused);
		SetSize(ImVec2(200.0f, 500.0f));
	}

	ListWidget::~ListWidget()
	{
	}

	void ListWidget::Update()
	{
		//ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 region = ImGui::GetContentRegionAvail();
		region.x += 18.0f;
		region.y += 3.0f;

		ImVec2 pos = ImGui::GetWindowPos();
		pos.x -= 0.0f;
		pos.y += 24.0f;

		ImGui::SetNextWindowPos(pos);

		if (ImGui::BeginListBox("##ListBox", region))
		{
			for (size_t i = 0; i < mResourceList.size(); ++i)
			{
				bool Selectable = (mListIndex == i);
				if (ImGui::Selectable(mResourceList[i].c_str(), Selectable))
				{
					mListIndex = i;
				}

				if (Selectable)
				{
					ImGui::SetItemDefaultFocus();
				}

				// 해당 아이템이 더블클릭 되었다.
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (mEventWidget && mEvent)
					{
						mEvent(mResourceList[i].c_str());
					}
					Close();
				}
			}

			ImGui::EndListBox();
		}
	}

	void ListWidget::LateUpdate()
	{
		
	}

	void ListWidget::Close()
	{
		SetState(eState::Paused);
		mResourceList.clear();

		WindowFocus(false);
	}

	void ListWidget::SetItemList(const std::vector<std::string>& _inList)
	{
		mResourceList = _inList;
	}

	void ListWidget::SetItemList(const std::vector<std::wstring>& _inList)
	{
		for (size_t i = 0; i < _inList.size(); ++i)
		{
			mResourceList.push_back(std::string(_inList[i].begin(), _inList[i].end()));
		}
	}
}
