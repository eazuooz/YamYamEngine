#include "guiProject.h"
#include "yaApplication.h"

extern ya::Application application;

namespace gui
{



	Project::Project()
	{
		SetName("Project");
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		Vector2 size = application.GetSize();

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));
	}

	Project::~Project()
	{
	}

	void Project::Update()
	{
		Widget::Update();
	}

	void Project::LateUpdate()
	{
	}

	void Project::Render()
	{
		Vector2 appPosition = application.GetPosition();
		Vector2 appSize = application.GetSize();

		ImGui::SetNextWindowPos(ImVec2(appPosition.x  - appSize.x / 5, appPosition.y + appSize.y));
		ImGui::SetNextWindowSize(GetSize());

		Widget::Render();
	}



}
