#include "guiProject.h"
#include "yaApplication.h"

extern ya::Application application;

namespace gui
{



	Project::Project()
		: mTreeWidget(nullptr)
	{
		SetName("Project");
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		Vector2 size = application.GetSize();

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		mTreeWidget = new TreeWidget();
		AddWidget(mTreeWidget);
	}

	Project::~Project()
	{
		//delete mTreeWidget;
		//mTreeWidget = nullptr;
	}

	void Project::FixedUpdate()
	{
		Widget::FixedUpdate();
	}

	void Project::Update()
	{
		Widget::Update();
	}

	void Project::LateUpdate()
	{
		
	}

}
