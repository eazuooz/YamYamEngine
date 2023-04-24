#include "guiEditor.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaGridScript.h"
#include "yaRenderer.h"
#include "BaseRenderer.h"
#include "yaApplication.h"
#include "yaGraphicsDevice_DX11.h"
#include "guiInspector.h"
#include "guiHierarchy.h"
#include "guiConsole.h"
#include "guiProject.h"
#include "guiListWidget.h"

extern ya::Application application;

namespace gui
{
	using namespace ya;
	void Editor::Initialize()
	{

#pragma region Debug
		mDebugObjects.resize((UINT)eColliderType::End);

		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"DebugRect");
		std::shared_ptr<Material> mateiral = Resources::Find<Material>(L"DebugMaterial");
		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>();
		mDebugObjects[(UINT)eColliderType::Rect]->GetComponent<MeshRenderer>()->SetMaterial(mateiral);
		mDebugObjects[(UINT)eColliderType::Rect]->GetComponent<MeshRenderer>()->SetMesh(mesh);


		std::shared_ptr<Mesh> circleMesh = Resources::Find<Mesh>(L"DebugCircle");
		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<MeshRenderer>();
		mDebugObjects[(UINT)eColliderType::Circle]->GetComponent<MeshRenderer>()->SetMaterial(mateiral);
		mDebugObjects[(UINT)eColliderType::Circle]->GetComponent<MeshRenderer>()->SetMesh(circleMesh);
#pragma endregion
#pragma region Editor
		//Grid 
		EditorObject* gridObj = new EditorObject();
		gridObj->SetName(L"Grid");
		gridObj->AddComponent<MeshRenderer>();

		GridScript* gridScript = gridObj->AddComponent<GridScript>();
		gridScript->SetCamera(renderer::mainCamera);

		MeshRenderer* girdMeshRenderer = gridObj->GetComponent<MeshRenderer>();
		girdMeshRenderer->SetMesh(Resources::Find<Mesh>(L"GridMesh"));
		girdMeshRenderer->SetMaterial(Resources::Find<Material>(L"GridMaterial"));
		//gridObj->GetComponent<Transform>()->SetPosition(0, 0, 20);

		mEditorObjects.push_back(gridObj);
#pragma endregion
#pragma region ImGui
		ImGui_Initialize();
#pragma endregion
#pragma region Widget
		// Create Widzet
		Inspector* inspector = new Inspector();
		mWidgets.insert(std::make_pair(inspector->GetName(), inspector));

		Hierarchy* hierarchy = new Hierarchy();
		mWidgets.insert(std::make_pair(hierarchy->GetName(), hierarchy));

		Project* project = new Project();
		mWidgets.insert(std::make_pair(project->GetName(), project));

		Console* console = new Console();
		mWidgets.insert(std::make_pair(console->GetName(), console));

		ListWidget* listWidget = new ListWidget();
		mWidgets.insert(std::make_pair(listWidget->GetName(), listWidget));
#pragma endregion
	}
	void Editor::Run()
	{
		Update();
		FixedUpdate();
		Render();

		ImGui_Run();
	}
	void Editor::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}
	void Editor::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->FixedUpdate();
		}
	}
	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}
		
		for (ya::graphics::DebugMesh& mesh : renderer::debugMeshes)
		{
			if (mesh.duration > 0.0f)
				continue;

			//mesh.position = Vector3(0.0f, 0.0f, 20.0f);
			DebugRender(mesh);
		}
		renderer::debugMeshes.clear();
	}

	void Editor::Release()
	{
		for (auto widget : mWidgets)
		{
			delete widget.second;
			widget.second = nullptr;
		}
		for (auto editorObject : mEditorObjects)
		{
			delete editorObject;
			editorObject = nullptr;
		}
		for (auto debugObject : mDebugObjects)
		{
			delete debugObject;
			debugObject = nullptr;
		}
	}

	void Editor::DebugRender(DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		Transform* tr = debugObj->GetComponent<Transform>();
		tr->SetPosition(mesh.position);
		tr->SetRotation(mesh.rotation);

		if (mesh.type == eColliderType::Rect)
		{
			tr->SetScale(mesh.scale);
		}
		else
		{
			tr->SetScale(Vector3(mesh.radius, mesh.radius, mesh.radius));
		}
		
		BaseRenderer* baseRenderer = debugObj->GetComponent<BaseRenderer>();
		Camera* mainCamera = renderer::mainCamera;

		tr->LateUpdate();

		Camera::SetGpuViewMatrix(renderer::mainCamera->GetGpuViewMatrix());
		Camera::SetGpuProjectionMatrix(renderer::mainCamera->GetGpuProjectionMatrix());

		debugObj->Render();
	}
	void Editor::ImGui_Initialize()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(application.GetHwnd());
		ImGui_ImplDX11_Init(ya::graphics::GetDevice()->GetID3D11Device()
			, ya::graphics::GetDevice()->GetID3D11DeviceContext());

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	}
	void Editor::ImGui_Run()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		bool show_demo_window = true;

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		for (auto& widget : mWidgets)
		{
			widget.second->Update();
		}
		for (auto& widget : mWidgets)
		{
			widget.second->Render();
		}

#pragma region SAMPLE
		//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		//{
		//	static float f = 0.0f;
		//	static int counter = 0;

		//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//	ImGui::Checkbox("Another Window", &show_another_window);

		//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//		counter++;
		//	ImGui::SameLine();
		//	ImGui::Text("counter = %d", counter);

		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//	ImGui::End();
		//}

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}
#pragma endregion

		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
	void Editor::ImGui_Release()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

