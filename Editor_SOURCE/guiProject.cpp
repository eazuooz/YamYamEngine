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
		
	}

	void Project::LateUpdate()
	{
	}

}
