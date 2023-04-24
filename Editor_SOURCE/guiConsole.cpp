#include "guiConsole.h"
#include "yaApplication.h"

extern ya::Application application;
namespace gui
{



	Console::Console()
	{
		SetName("Console");
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		Vector2 size = application.GetSize();

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));
	}

	Console::~Console()
	{
	}

	void Console::Update()
	{
		Widget::Update();
	}

	void Console::LateUpdate()
	{
	}

	void Console::Render()
	{
		Vector2 appPosition = application.GetPosition();
		Vector2 appSize = application.GetSize();

		ImGui::SetNextWindowPos(ImVec2(appPosition.x + appSize.x / 2, appPosition.y + appSize.y));
		ImGui::SetNextWindowSize(GetSize());

		Widget::Render();
	}
}