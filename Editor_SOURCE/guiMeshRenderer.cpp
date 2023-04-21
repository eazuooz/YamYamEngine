#include "guiMeshRenderer.h"
#include "yaGameObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"

#include "guiListWidget.h"
#include "yaResources.h"
#include "guiEditor.h"


extern gui::Editor editor;
namespace gui
{

	MeshRenderer::MeshRenderer()
		: Component(ya::enums::eComponentType::MeshRenderer)
	{
		SetName("MeshRenderer");
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::Update()
	{
		if (GetTarget() != nullptr)
		{
			ya::MeshRenderer* meshRenderer = GetTarget()->GetComponent<ya::MeshRenderer>();

			mMesh = meshRenderer->GetMesh();
			mMaterial = meshRenderer->GetMaterial();
		}

		Component::Update();
	}

	void MeshRenderer::LateUpdate()
	{
		Component::LateUpdate();

		std::string meshName 
			= std::string(mMesh->GetKey().begin(), mMesh->GetKey().end());
		std::string materialName
			= std::string(mMaterial->GetKey().begin(), mMaterial->GetKey().end());

		ImGui::Text("Mesh     "); 
		ImGui::SameLine(); 
		ImGui::InputText("##MeshName", (char*)meshName.data(), meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		if (ImGui::Button("##MeshBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* pListUI = editor.GetWidget<ListWidget>("ListWidget");
			assert(pListUI);

			std::vector<std::shared_ptr<ya::Mesh>> meshes
				= ya::Resources::Finds<ya::Mesh>();

			std::vector<std::string> materialNames;
			for (std::shared_ptr<ya::Mesh>  mesh : meshes)
			{
				std::string name(mesh->GetName().begin(), mesh->GetName().end());
				materialNames.push_back(name);
			}
			pListUI->SetItemList(materialNames);
			pListUI->SetEvent(this, std::bind(&MeshRenderer::SetMesh, this, std::placeholders::_1));

			pListUI->SetState(eState::Active);
		}

		ImGui::Text("Material "); ImGui::SameLine(); ImGui::InputText("##MtrlName", (char*)materialName.data(), materialName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		if (ImGui::Button("##MtrlBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* pListUI = editor.GetWidget<ListWidget>("ListWidget");
			assert(pListUI);

			std::vector<std::shared_ptr<ya::Material>> materials
				= ya::Resources::Finds<ya::Material>();

			std::vector<std::string> materialNames;
			for (std::shared_ptr<ya::Material> material : materials)
			{
				std::string name(material->GetName().begin(), material->GetName().end());
				materialNames.push_back(name);
			}
			pListUI->SetItemList(materialNames);
			pListUI->SetEvent(this, std::bind(&MeshRenderer::SetMaterial, this, std::placeholders::_1));

			pListUI->SetState(eState::Active);
		}
	}

	void MeshRenderer::SetMesh(std::string key)
	{
		//std::string key = (char*)name;
		std::wstring wKey(key.begin(), key.end());
		std::shared_ptr<ya::Mesh> mesh = ya::Resources::Find<ya::Mesh>(wKey);

		GetTarget()->GetComponent<ya::MeshRenderer>()->SetMesh(mesh);
	}

	void MeshRenderer::SetMaterial(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		std::shared_ptr<ya::Material> material = ya::Resources::Find<ya::Material>(wKey);

		GetTarget()->GetComponent<ya::MeshRenderer>()->SetMaterial(material);
	}
}
