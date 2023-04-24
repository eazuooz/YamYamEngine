#include "guiHierarchy.h"


namespace gui
{



	Hierarchy::Hierarchy()
	{
		SetName("Hierarchy");
		SetSize(ImVec2(100.0f, 150.0f));

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
		Widget::Render();
	}

}
