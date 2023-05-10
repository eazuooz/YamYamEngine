#include "guiProject.h"
#include "yaApplication.h"
#include "guiTreeWidget.h"
#include "YamYamEngine.h"
#include "yaMesh.h"
#include "yaTexture.h"
#include "yaMaterial.h"
#include "yaShader.h"
#include "guiEditor.h"
#include "guiInspector.h"

extern ya::Application application;
extern gui::Editor editor;

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
		mTreeWidget->SetName("Resources");
		AddWidget(mTreeWidget);
		mTreeWidget->SetEvent(this, std::bind(&Project::toInspector
			, this, std::placeholders::_1));

		ResetContent();
	}

	Project::~Project()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}

	void Project::FixedUpdate()
	{
		Widget::FixedUpdate();

		//리소스가 바뀐다면 리소스목록 초기화
	}

	void Project::Update()
	{
		Widget::Update();
	}

	void Project::LateUpdate()
	{
		
	}

	void Project::ResetContent()
	{
		mTreeWidget->Clear();
		TreeWidget::Node* rootNode = mTreeWidget->AddNode(nullptr, "Resources", 0, true);

		//enum class eResourceType
		//{
		//	Mesh,
		//	Texture,
		//	Material,
		//	Sound,
		//	Prefab,
		//	MeshData,
		//	GraphicsShader,
		//	ComputeShader,
		//	End,
		//};
		AddResources<ya::Mesh>(rootNode, "Mesh");
		AddResources<ya::graphics::Texture>(rootNode, "Texture");
		AddResources<ya::graphics::Material>(rootNode, "Materials");
		AddResources<ya::graphics::Shader>(rootNode, "Shaders");
	}

	void Project::toInspector(const std::string& name)
	{
		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		std::wstring wName(name.begin(), name.end());

		
	}

}
