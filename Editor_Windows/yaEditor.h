#pragma once
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaGraphics.h"
#include "yaWidget.h"
#include "yaDebugObject.h"
#include "yaEditorObject.h"

namespace ya
{
	using namespace ya::graphics;
	class Editor
	{
	public:
		static void Initialize();
		static void Run();
		
		static void Update();
		static void FixedUpdate();
		static void Render();
		static void Release();

		static void DebugRender(DebugMesh& mesh);

	private:
		static std::vector<gui::Widget*> mWidgets;
		static std::vector<EditorObject*> mEditorObjects;
		static std::vector<DebugObject*> mDebugObjects;
	};
}
