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

		SetSize(ImVec2((float)width / 2 + width / 5, height / 4));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Resources");
		AddWidget(mTreeWidget);
		mTreeWidget->SetEvent(this, std::bind(&Project::SelectResource
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

		//���ҽ��� �ٲ�ٸ� ���ҽ���� �ʱ�ȭ
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

	void Project::SelectResource(void* data)
	{
		ya::Resource* resource = static_cast<ya::Resource*>(data);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		std::wstring wName = resource->GetName();

		inspector->InitializeTarget(resource);
	}

}
