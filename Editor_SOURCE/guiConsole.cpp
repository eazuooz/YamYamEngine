#include "guiConsole.h"

namespace gui
{



	Console::Console()
	{
		SetName("Console");
		SetSize(ImVec2(100.0f, 150.0f));

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
		Widget::Render();
	}

}
