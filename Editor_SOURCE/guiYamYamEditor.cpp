#include "guiYamYamEditor.h"
#include "yaApplication.h"
#include "guiEditor.h"

extern ya::Application application;
extern gui::Editor editor;
namespace gui
{



	YamYamEditor::YamYamEditor()
	{
		SetName("YamYamEditor");

		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		Vector2 size = application.GetSize();

		SetSize(ImVec2((float)size.x + size.x / 2.5f, (float)50));

		//ImGui::Begin("VXR", 0, ImGuiWindowFlags_MenuBar
		//	| ImGuiWindowFlags_NoTitleBar
		//	| ImGuiWindowFlags_NoResize
		//	| ImGuiWindowFlags_NoMove
		//	| ImGuiWindowFlags_NoScrollbar
		//	| ImGuiWindowFlags_NoScrollWithMouse
		//	| ImGuiWindowFlags_NoCollapse
		//	| ImGuiWindowFlags_AlwaysAutoResize
		//	| ImGuiWindowFlags_NoBringToFrontOnFocus);

		SetFlag(ImGuiWindowFlags_MenuBar);
			//| ImGuiWindowFlags_NoTitleBar
			//| ImGuiWindowFlags_NoResize
			//| ImGuiWindowFlags_NoMove
			//| ImGuiWindowFlags_NoScrollbar
			//| ImGuiWindowFlags_NoScrollWithMouse
			//| ImGuiWindowFlags_NoCollapse
			//| ImGuiWindowFlags_AlwaysAutoResize
			//| ImGuiWindowFlags_NoBringToFrontOnFocus);

	}

	YamYamEditor::~YamYamEditor()
	{
	}

	void YamYamEditor::Update()
	{

		

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		Widget::Update();
	}

	void YamYamEditor::LateUpdate()
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("View"))
		{
			bool show_statistics = editor.GetEnable();
			ImGui::MenuItem("Show Statistics", "", &show_statistics);
			editor.SetEnable(show_statistics);

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	void YamYamEditor::Render()
	{
		Vector2 appPosition = application.GetPosition();
		Vector2 size = application.GetSize();

		ImGui::SetNextWindowPos(ImVec2(appPosition.x - size.x / 5, appPosition.y - GetSize().y));
		ImGui::SetNextWindowSize(GetSize());

		Widget::Render();
	}



}