#include "guiProject.h"


namespace gui
{



	Project::Project()
	{
		SetName("Project");
		SetSize(ImVec2(100.0f, 150.0f));

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
		Widget::Render();
	}



}
