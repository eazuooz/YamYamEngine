#include "guiInspector.h"
#include "guiTransform.h"
#include "guiMeshRenderer.h"
#include "yaRenderer.h"
#include "yaApplication.h"
#include "guiTexture.h"

extern ya::Application application;
namespace gui
{
	using namespace ya::enums;

	Inspector::Inspector()
		: mTargetGameObject(nullptr)
		, mTargetResource(nullptr)
	{
		SetName("Inspector");
		UINT height = application.GetHeight();
		Vector2 size = application.GetSize();
		SetSize(ImVec2(size.x / 5, size.y));

		mTargetGameObject = ya::renderer::inspectorGameObject;
		
		mComponents.resize((UINT)eComponentType::End);

		mComponents[(UINT)eComponentType::Transform] = new gui::Transform();
		mComponents[(UINT)eComponentType::Transform]->SetSize(ImVec2(0.0f, 150.0f));
		mComponents[(UINT)eComponentType::MeshRenderer] = new gui::MeshRenderer();
		mComponents[(UINT)eComponentType::MeshRenderer]->SetSize(ImVec2(0.0f, 150.0f));
		
		AddWidget(mComponents[(UINT)eComponentType::Transform]);
		AddWidget(mComponents[(UINT)eComponentType::MeshRenderer]);

		mResources.resize((UINT)eResourceType::End);
		mResources[(UINT)eResourceType::Texture] = new gui::Texture();
		mResources[(UINT)eResourceType::Texture]->SetName("InspectorTexture");
		
		AddWidget(mResources[(UINT)eResourceType::Texture]);

		if (mTargetGameObject == nullptr)
			return;

		InitializeTarget(mTargetGameObject);
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

		for (gui::Resource* comp : mResources)
		{
			if (comp == nullptr)
				continue;

			delete comp;
			comp = nullptr;
		}
	}

	void Inspector::Update()
	{
		
	}

	void Inspector::LateUpdate()
	{

	}

	void Inspector::ClearTarget()
	{
		for (size_t i = 0; i < (UINT)eComponentType::End; i++)
		{
			if (mComponents[i] == nullptr)
				continue;

			mComponents[i]->SetState(eState::Paused);
			mComponents[i]->SetTarget(nullptr);
		}

		for (size_t i = 0; i < (UINT)eResourceType::End; i++)
		{
			if (mResources[i] == nullptr)
				continue;

			mResources[i]->SetState(eState::Paused);
			mResources[i]->SetTarget(nullptr);
		}
	}

	void Inspector::InitializeTarget(ya::GameObject* gameObj)
	{
		ClearTarget();

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

	void Inspector::InitializeTarget(ya::Resource* resource)
	{
		ClearTarget();

		eResourceType type = resource->GetType();
		switch (type)
		{
		case ya::enums::eResourceType::Mesh:
			break;
		case ya::enums::eResourceType::Texture:
			{
				mResources[(UINT)eResourceType::Texture]->SetTarget(resource);
				mResources[(UINT)eResourceType::Texture]->SetState(eState::Active);
			}
			break;
		case ya::enums::eResourceType::Material:
			break;
		case ya::enums::eResourceType::Sound:
			break;
		case ya::enums::eResourceType::Prefab:
			break;
		case ya::enums::eResourceType::MeshData:
			break;
		case ya::enums::eResourceType::GraphicsShader:
			break;
		case ya::enums::eResourceType::ComputeShader:
			break;
		case ya::enums::eResourceType::End:
			break;
		default:
			break;
		}
	}

}
