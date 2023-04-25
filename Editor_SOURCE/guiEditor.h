#pragma once
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaGraphics.h"
#include "guiWidget.h"
#include "guiDebugObject.h"
#include "guiEditorObject.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
	using namespace ya::graphics;
	class Editor
	{
	public:
		template <typename T>
		T* GetWidget(const std::string& name)
		{
			auto iter = mWidgets.find(name);
			if (iter == mWidgets.end())
				return nullptr;

			return dynamic_cast<T*>(iter->second);
		}

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

		void SetEnable(bool enable) { mEnable = enable; }
		bool GetEnable() { return mEnable; }

	private:
		std::map<std::string, gui::Widget*> mWidgets;
		std::vector<gui::EditorObject*> mEditorObjects;
		std::vector<gui::DebugObject*> mDebugObjects;
		bool mEnable;

		class YamYamEditor* mYamYamEditor;
	};
}
