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

	}

	void Console::LateUpdate()
	{
	}


}