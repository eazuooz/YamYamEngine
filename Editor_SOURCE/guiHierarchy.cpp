#include "guiHierarchy.h"
#include "yaApplication.h"
#include "yaScene.h"
#include "yaLayer.h"
#include "yaSceneManager.h"
#include "guiInspector.h"
#include "guiEditor.h"

extern ya::Application application;
extern gui::Editor editor;

namespace gui
{



	Hierarchy::Hierarchy()
		: mTreeWidget(nullptr)
	{
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		SetName("Hierarchy");
		SetSize(ImVec2(width / 5, height));


		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Scenes");
		AddWidget(mTreeWidget);
		mTreeWidget->SetEvent(this, std::bind(&Hierarchy::SelectGameObject
			, this, std::placeholders::_1));

		//
		//mTreeWidget->SetState()
		InitializeScene();
	}

	Hierarchy::~Hierarchy()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}

	void Hierarchy::Update()
	{
		Widget::Update();
	}

	void Hierarchy::LateUpdate()
	{
	}

	void Hierarchy::InitializeInspector(void* data)
	{

	}

	void Hierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		ya::Scene* scene = ya::SceneManager::GetActiveScene();
		std::string name(scene->GetName().begin(), scene->GetName().end());

		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, name, 0, true);

		for (size_t i = 0; i < (UINT)ya::enums::eLayerType::Max; i++)
		{
			ya::Layer* layer = scene->GetLayer((ya::enums::eLayerType)i);
			const std::vector<ya::GameObject*> gameObjs = layer->GetGameObjects();
			for (ya::GameObject* obj : gameObjs)
			{
				AddGameObject(root, obj);
			}
		}
	}

	void Hierarchy::AddGameObject(TreeWidget::Node* parent, ya::GameObject* gameObject)
	{
		std::string name(gameObject->GetName().begin(), gameObject->GetName().end());

		//if (name == "")
		//	return;

		TreeWidget::Node* curNode 
			= mTreeWidget->AddNode(parent, name.c_str(), gameObject);
	}

	void Hierarchy::SelectGameObject(void* data)
	{
		ya::GameObject* gameObject = static_cast<ya::GameObject*>(data);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		std::wstring wName = gameObject->GetName();

		ya::Scene* scene = ya::SceneManager::GetActiveScene();
		for (size_t i = 0; i < (UINT)ya::enums::eLayerType::Max; i++)
		{
			ya::Layer* layer = scene->GetLayer((ya::enums::eLayerType)i);
			const std::vector<ya::GameObject*> gameObjs = layer->GetGameObjects();
			for (ya::GameObject* obj : gameObjs)
			{
				if (obj->GetName() == wName)
				{
					inspector->SetTargetGameObject(obj);
					inspector->InitializeTarget(obj);
				}
			}
		}
	}
}
