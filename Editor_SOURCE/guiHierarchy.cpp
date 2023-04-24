#include "guiHierarchy.h"
#include "yaApplication.h"

extern ya::Application application;

namespace gui
{



	Hierarchy::Hierarchy()
	{
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();
		Vector2 size = application.GetSize();

		SetName("Hierarchy");
		SetSize(ImVec2(size.x / 5, size.y));

	}

	Hierarchy::~Hierarchy()
	{
	}

	void Hierarchy::Update()
	{
		Widget::Update();
	}

	void Hierarchy::LateUpdate()
	{
	}

	void Hierarchy::Render()
	{
		Vector2 appPosition = application.GetPosition();

		ImGui::SetNextWindowPos(ImVec2(appPosition.x - GetSize().x + 8.0f, appPosition.y));
		ImGui::SetNextWindowSize(GetSize());

		Widget::Render();
	}

}
