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

		SetSize(ImVec2((float)width / 2 + width / 5, height / 4));
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