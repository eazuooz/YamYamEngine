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

	void ListWidget::LateUpdate()
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
			bool Selectable = false;
			ImGui::Selectable("Item 01", &Selectable);

			ImGui::EndListBox();
		}
	}
}
