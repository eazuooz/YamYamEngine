#include "guiProject.h"
#include "yaApplication.h"
#include "guiTreeWidget.h"
#include "YamYamEngine.h"
#include "yaMesh.h"
#include "yaTexture.h"
#include "yaMaterial.h"
#include "yaShader.h"

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
		mTreeWidget->SetName("Resources");
		AddWidget(mTreeWidget);

		mTreeWidget->SetDummyRoot(true);
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
		//mTreeWidget->Close();
		mTreeWidget->Clear();

		TreeWidget::Node* pRootNode = mTreeWidget->AddNode(nullptr, "Resources", 0, true);

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
		AddResources<ya::Mesh>(pRootNode, "Mesh");
		AddResources<ya::graphics::Texture>(pRootNode, "Texture");
		AddResources<ya::graphics::Material>(pRootNode, "Materials");
		AddResources<ya::graphics::Shader>(pRootNode, "Shaders");
	}

	void Project::toInspector()
	{

	}

}
