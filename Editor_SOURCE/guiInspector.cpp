#include "guiInspector.h"
#include "guiTransform.h"
#include "guiMeshRenderer.h"
#include "yaRenderer.h"
#include "yaApplication.h"

extern ya::Application application;
namespace gui
{
	using namespace ya::enums;

	Inspector::Inspector()
		: mTarget(nullptr)
	{
		SetName("Inspector");
		mTarget = ya::renderer::inspectorGameObject;
		mComponents.resize((UINT)eComponentType::End);

		UINT height = application.GetHeight();
		Vector2 size = application.GetSize();
		SetSize(ImVec2(size.x / 5, size.y));

		mComponents[(UINT)eComponentType::Transform] = new gui::Transform();
		mComponents[(UINT)eComponentType::Transform]->SetSize(ImVec2(0.0f, 150.0f));

		AddWidget(mComponents[(UINT)eComponentType::Transform]);

		mComponents[(UINT)eComponentType::MeshRenderer] = new gui::MeshRenderer();
		mComponents[(UINT)eComponentType::MeshRenderer]->SetSize(ImVec2(0.0f, 150.0f));
		AddWidget(mComponents[(UINT)eComponentType::MeshRenderer]);
	}

	Inspector::~Inspector()
	{
		for (gui::Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			delete comp;
			comp = nullptr;
		}
	}

	void Inspector::Update()
	{
		if (mTarget == nullptr)
			return;

		InitializeTarget(mTarget);
	}

	void Inspector::LateUpdate()
	{

	}

	void Inspector::InitializeTarget(ya::GameObject* gameObj)
	{
		std::vector<ya::Component*>& components = gameObj->GetComponents();
		for (size_t i = 0; i < (UINT)eComponentType::End; i++)
		{
			ya::Component* comp = components[i];
			if (comp == nullptr)
				continue;
			if (mComponents[i] == nullptr)
				continue;

			if (comp == nullptr)
			{
				mComponents[i]->SetTarget(nullptr);
				mComponents[i]->SetState(eState::Paused);
			}
			else
			{
				mComponents[i]->SetTarget(gameObj);
				mComponents[i]->SetState(eState::Active);
			}
		}
	}

}
