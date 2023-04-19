#include "guiMeshRenderer.h"
#include "yaGameObject.h"
#include "yaMeshRenderer.h"

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

			pListUI->SetState(eState::Active);
		}

		ImGui::Text("Material "); ImGui::SameLine(); ImGui::InputText("##MtrlName", (char*)materialName.data(), materialName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		if (ImGui::Button("##MtrlBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* pListUI = editor.GetWidget<ListWidget>("ListWidget");
			assert(pListUI);

			pListUI->SetState(eState::Active);
		}
	}
}
