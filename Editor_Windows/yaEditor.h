#pragma once
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaGraphics.h"
#include "yaWidget.h"
#include "yaDebugObject.h"
#include "yaEditorObject.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
	using namespace ya::graphics;
	class Editor
	{
	public:
		void Initialize();
		void Run();
		
		void Update();
		void FixedUpdate();
		void Render();
		void Release();

		void DebugRender(DebugMesh& mesh);

		void ImGui_Initialize();
		void ImGui_Run();
		void ImGui_Release();

	private:
		std::map<std::string, gui::Widget*> mWidgets;
		std::vector<gui::EditorObject*> mEditorObjects;
		std::vector<gui::DebugObject*> mDebugObjects;
	};
}
